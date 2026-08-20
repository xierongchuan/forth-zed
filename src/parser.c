/*
 * OOS Forth Tree-sitter parser.
 *
 * Forth remains deliberately flat at the editor level, with one exception:
 * parenthesized stack effects are split into delimiters, parameter text, and
 * the "--" separator. This lets Zed distinguish stack-effect parameters from
 * ordinary comments without pretending to understand run-time Forth semantics.
 *
 * ABI: Tree-sitter language ABI 15.
 */
#include "tree_sitter/parser.h"

#define LANGUAGE_VERSION 15
#define STATE_COUNT 18
#define LARGE_STATE_COUNT 18
#define SYMBOL_COUNT 15
#define ALIAS_COUNT 0
#define TOKEN_COUNT 11
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 0
#define MAX_ALIAS_SEQUENCE_LENGTH 1
#define MAX_RESERVED_WORD_SET_SIZE 0
#define PRODUCTION_ID_COUNT 1
#define SUPERTYPE_COUNT 0

enum ts_symbol_identifiers {
  sym_line_comment = 1,
  sym_paren_delimiter = 2,
  sym_paren_content = 3,
  sym_stack_effect_separator = 4,
  sym_string = 5,
  sym_number = 6,
  sym_character_literal = 7,
  sym_colon = 8,
  sym_semicolon = 9,
  sym_word = 10,
  sym_paren_comment = 11,
  sym_stack_effect = 12,
  sym_source_file = 13,
  aux_sym_source_file_repeat1 = 14,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [sym_line_comment] = "line_comment",
  [sym_paren_delimiter] = "paren_delimiter",
  [sym_paren_content] = "paren_content",
  [sym_stack_effect_separator] = "stack_effect_separator",
  [sym_string] = "string",
  [sym_number] = "number",
  [sym_character_literal] = "character_literal",
  [sym_colon] = "colon",
  [sym_semicolon] = "semicolon",
  [sym_word] = "word",
  [sym_paren_comment] = "paren_comment",
  [sym_stack_effect] = "stack_effect",
  [sym_source_file] = "source_file",
  [aux_sym_source_file_repeat1] = "source_file_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [sym_line_comment] = sym_line_comment,
  [sym_paren_delimiter] = sym_paren_delimiter,
  [sym_paren_content] = sym_paren_content,
  [sym_stack_effect_separator] = sym_stack_effect_separator,
  [sym_string] = sym_string,
  [sym_number] = sym_number,
  [sym_character_literal] = sym_character_literal,
  [sym_colon] = sym_colon,
  [sym_semicolon] = sym_semicolon,
  [sym_word] = sym_word,
  [sym_paren_comment] = sym_paren_comment,
  [sym_stack_effect] = sym_stack_effect,
  [sym_source_file] = sym_source_file,
  [aux_sym_source_file_repeat1] = aux_sym_source_file_repeat1,
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {.visible = false, .named = true},
  [sym_line_comment] = {.visible = true, .named = true},
  [sym_paren_delimiter] = {.visible = true, .named = true},
  [sym_paren_content] = {.visible = true, .named = true},
  [sym_stack_effect_separator] = {.visible = true, .named = true},
  [sym_string] = {.visible = true, .named = true},
  [sym_number] = {.visible = true, .named = true},
  [sym_character_literal] = {.visible = true, .named = true},
  [sym_colon] = {.visible = true, .named = true},
  [sym_semicolon] = {.visible = true, .named = true},
  [sym_word] = {.visible = true, .named = true},
  [sym_paren_comment] = {.visible = true, .named = true},
  [sym_stack_effect] = {.visible = true, .named = true},
  [sym_source_file] = {.visible = true, .named = true},
  [aux_sym_source_file_repeat1] = {.visible = false, .named = false},
};

static const TSSymbol ts_alias_sequences[PRODUCTION_ID_COUNT][MAX_ALIAS_SEQUENCE_LENGTH] = {
  [0] = {0},
};

static const TSStateId ts_primary_state_ids[STATE_COUNT] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
};

enum forth_lex_state {
  LEX_TOP = 0,
  LEX_PAREN_LEFT = 1,
  LEX_PAREN_AFTER_LEFT = 2,
  LEX_PAREN_RIGHT = 3,
  LEX_PAREN_CLOSE = 4,
};

static inline bool forth_space(int32_t c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

static inline void skip_space(TSLexer *lexer) {
  while (!lexer->eof(lexer) && forth_space(lexer->lookahead)) {
    lexer->advance(lexer, true);
  }
}

static inline bool ascii_digit(char c) { return c >= '0' && c <= '9'; }
static inline bool ascii_hex(char c) {
  return ascii_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static bool token_is_number(const char *s, unsigned n) {
  if (n == 0) return false;
  unsigned i = 0;
  if (s[i] == '+' || s[i] == '-') {
    i++;
    if (i == n) return false;
  }

  if (s[i] == '$') {
    if (++i == n) return false;
    for (; i < n; i++) if (!ascii_hex(s[i])) return false;
    return true;
  }
  if (s[i] == '%') {
    if (++i == n) return false;
    for (; i < n; i++) if (s[i] != '0' && s[i] != '1') return false;
    return true;
  }
  if (s[i] == '&') {
    if (++i == n) return false;
    for (; i < n; i++) if (s[i] < '0' || s[i] > '7') return false;
    return true;
  }
  if (i + 2 <= n && s[i] == '0' && i + 1 < n && (s[i + 1] == 'x' || s[i + 1] == 'X')) {
    i += 2;
    if (i == n) return false;
    for (; i < n; i++) if (!ascii_hex(s[i])) return false;
    return true;
  }

  bool any_digit = false;
  while (i < n && ascii_digit(s[i])) { any_digit = true; i++; }
  if (!any_digit) return false;

  if (i < n && s[i] == '.') {
    i++;
    while (i < n && ascii_digit(s[i])) i++;
  }

  if (i < n && (s[i] == 'e' || s[i] == 'E')) {
    i++;
    if (i < n && (s[i] == '+' || s[i] == '-')) i++;
    unsigned exponent_start = i;
    while (i < n && ascii_digit(s[i])) i++;
    if (i == exponent_start) return false;
  }
  return i == n;
}

static bool is_string_opener(const char *s, unsigned n) {
  if (n == 2 && s[0] == '.' && s[1] == '"') return true;
  if (n == 2 && (s[0] == 's' || s[0] == 'S' || s[0] == 'c' || s[0] == 'C') && s[1] == '"') return true;
  if (n == 6 && (s[0] == 'a' || s[0] == 'A') &&
      (s[1] == 'b' || s[1] == 'B') && (s[2] == 'o' || s[2] == 'O') &&
      (s[3] == 'r' || s[3] == 'R') && (s[4] == 't' || s[4] == 'T') && s[5] == '"') return true;
  return false;
}

static bool emit_end(TSLexer *lexer) {
  lexer->result_symbol = ts_builtin_sym_end;
  lexer->mark_end(lexer);
  return true;
}

static bool emit_paren_delimiter(TSLexer *lexer) {
  lexer->advance(lexer, false);
  lexer->mark_end(lexer);
  lexer->result_symbol = sym_paren_delimiter;
  return true;
}

/*
 * Scan the text between '(' and either '--' or ')'. The token may contain
 * spaces and nested parenthesized notation such as fib(n). Trailing spaces
 * are intentionally left outside the token, so only meaningful parameter or
 * comment text receives a semantic highlight.
 */
static bool lex_paren_content(TSLexer *lexer, bool stop_at_separator) {
  skip_space(lexer);
  if (lexer->eof(lexer)) return emit_end(lexer);
  if (lexer->lookahead == ')') return emit_paren_delimiter(lexer);

  bool started = false;
  int depth = 0;

  while (!lexer->eof(lexer)) {
    int32_t c = lexer->lookahead;

    if (depth == 0 && c == ')') break;

    if (depth == 0 && stop_at_separator && c == '-') {
      lexer->advance(lexer, false);
      if (lexer->lookahead == '-') {
        if (!started) {
          lexer->advance(lexer, false);
          lexer->mark_end(lexer);
          lexer->result_symbol = sym_stack_effect_separator;
          return true;
        }
        lexer->advance(lexer, false);
        lexer->result_symbol = sym_paren_content;
        return true;
      }
      started = true;
      lexer->mark_end(lexer);
      continue;
    }

    if (c == '(') {
      depth++;
      started = true;
      lexer->advance(lexer, false);
      lexer->mark_end(lexer);
      continue;
    }
    if (c == ')' && depth > 0) {
      depth--;
      started = true;
      lexer->advance(lexer, false);
      lexer->mark_end(lexer);
      continue;
    }

    lexer->advance(lexer, false);
    if (!forth_space(c)) {
      started = true;
      lexer->mark_end(lexer);
    }
  }

  if (started) {
    lexer->result_symbol = sym_paren_content;
    return true;
  }
  if (!lexer->eof(lexer) && lexer->lookahead == ')') return emit_paren_delimiter(lexer);
  return emit_end(lexer);
}

static bool lex_after_left(TSLexer *lexer) {
  skip_space(lexer);
  if (lexer->eof(lexer)) return emit_end(lexer);
  if (lexer->lookahead == ')') return emit_paren_delimiter(lexer);
  if (lexer->lookahead == '-') {
    lexer->advance(lexer, false);
    if (lexer->lookahead == '-') {
      lexer->advance(lexer, false);
      lexer->mark_end(lexer);
      lexer->result_symbol = sym_stack_effect_separator;
      return true;
    }
  }
  return false;
}

static bool lex_close(TSLexer *lexer) {
  skip_space(lexer);
  if (lexer->eof(lexer)) return emit_end(lexer);
  if (lexer->lookahead == ')') return emit_paren_delimiter(lexer);
  return false;
}

static bool lex_top(TSLexer *lexer) {
  skip_space(lexer);
  if (lexer->eof(lexer)) return emit_end(lexer);

  if (lexer->lookahead == '\\') {
    do {
      lexer->advance(lexer, false);
      lexer->mark_end(lexer);
    } while (!lexer->eof(lexer) && lexer->lookahead != '\n' && lexer->lookahead != '\r');
    lexer->result_symbol = sym_line_comment;
    return true;
  }

  if (lexer->lookahead == '(') return emit_paren_delimiter(lexer);

  char buf[256];
  unsigned len = 0;
  bool ascii = true;

  while (!lexer->eof(lexer) && !forth_space(lexer->lookahead)) {
    int32_t c = lexer->lookahead;
    if (c < 0 || c > 127) ascii = false;
    if (len < sizeof(buf)) buf[len] = (char)c;
    len++;
    lexer->advance(lexer, false);
    lexer->mark_end(lexer);

    if (ascii && len <= sizeof(buf) && is_string_opener(buf, len)) {
      while (!lexer->eof(lexer)) {
        int32_t sc = lexer->lookahead;
        lexer->advance(lexer, false);
        lexer->mark_end(lexer);
        if (sc == '"') break;
      }
      lexer->result_symbol = sym_string;
      return true;
    }
  }

  if (ascii && len <= sizeof(buf)) {
    if (len == 1 && buf[0] == ':') { lexer->result_symbol = sym_colon; return true; }
    if (len == 1 && buf[0] == ';') { lexer->result_symbol = sym_semicolon; return true; }
    if (len == 3 && buf[0] == '\'' && buf[2] == '\'') { lexer->result_symbol = sym_character_literal; return true; }
    if (token_is_number(buf, len)) { lexer->result_symbol = sym_number; return true; }
  }

  lexer->result_symbol = sym_word;
  return true;
}

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  switch (state) {
    case LEX_PAREN_LEFT: return lex_paren_content(lexer, true);
    case LEX_PAREN_AFTER_LEFT: return lex_after_left(lexer);
    case LEX_PAREN_RIGHT: return lex_paren_content(lexer, false);
    case LEX_PAREN_CLOSE: return lex_close(lexer);
    case LEX_TOP:
    default: return lex_top(lexer);
  }
}

static const TSLexerMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = LEX_TOP},
  [1] = {.lex_state = LEX_TOP},
  [2] = {.lex_state = LEX_TOP},
  [3] = {.lex_state = LEX_TOP},
  [4] = {.lex_state = LEX_TOP},
  [5] = {.lex_state = LEX_TOP},
  [6] = {.lex_state = LEX_PAREN_LEFT},
  [7] = {.lex_state = LEX_TOP},
  [8] = {.lex_state = LEX_PAREN_AFTER_LEFT},
  [9] = {.lex_state = LEX_PAREN_RIGHT},
  [10] = {.lex_state = LEX_TOP},
  [11] = {.lex_state = LEX_PAREN_RIGHT},
  [12] = {.lex_state = LEX_TOP},
  [13] = {.lex_state = LEX_PAREN_CLOSE},
  [14] = {.lex_state = LEX_TOP},
  [15] = {.lex_state = LEX_PAREN_CLOSE},
  [16] = {.lex_state = LEX_TOP},
  [17] = {.lex_state = LEX_TOP},
};

#define FORTH_SIMPLE_TERMINAL_ACTIONS(action) \
  [sym_line_comment] = ACTIONS(action), \
  [sym_string] = ACTIONS(action), \
  [sym_number] = ACTIONS(action), \
  [sym_character_literal] = ACTIONS(action), \
  [sym_colon] = ACTIONS(action), \
  [sym_semicolon] = ACTIONS(action), \
  [sym_word] = ACTIONS(action)

#define FORTH_TOP_LEVEL_FOLLOW_ACTIONS(action) \
  [ts_builtin_sym_end] = ACTIONS(action), \
  [sym_paren_delimiter] = ACTIONS(action), \
  FORTH_SIMPLE_TERMINAL_ACTIONS(action)

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [STATE(0)] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [sym_paren_delimiter] = ACTIONS(1),
    [sym_paren_content] = ACTIONS(1),
    [sym_stack_effect_separator] = ACTIONS(1),
    FORTH_SIMPLE_TERMINAL_ACTIONS(1),
  },
  [STATE(1)] = {
    [sym_source_file] = STATE(5),
    [aux_sym_source_file_repeat1] = STATE(2),
    [sym_paren_comment] = STATE(3),
    [sym_stack_effect] = STATE(3),
    [ts_builtin_sym_end] = ACTIONS(3),
    [sym_paren_delimiter] = ACTIONS(7),
    FORTH_SIMPLE_TERMINAL_ACTIONS(5),
  },
  [STATE(2)] = {
    [sym_paren_comment] = STATE(4),
    [sym_stack_effect] = STATE(4),
    [ts_builtin_sym_end] = ACTIONS(9),
    [sym_paren_delimiter] = ACTIONS(7),
    FORTH_SIMPLE_TERMINAL_ACTIONS(11),
  },
  [STATE(3)] = {
    FORTH_TOP_LEVEL_FOLLOW_ACTIONS(13),
  },
  [STATE(4)] = {
    FORTH_TOP_LEVEL_FOLLOW_ACTIONS(15),
  },
  [STATE(5)] = {
    [ts_builtin_sym_end] = ACTIONS(17),
  },
  [STATE(6)] = {
    [sym_paren_delimiter] = ACTIONS(19),
    [sym_paren_content] = ACTIONS(21),
    [sym_stack_effect_separator] = ACTIONS(23),
  },
  [STATE(7)] = {
    FORTH_TOP_LEVEL_FOLLOW_ACTIONS(25),
  },
  [STATE(8)] = {
    [sym_paren_delimiter] = ACTIONS(27),
    [sym_stack_effect_separator] = ACTIONS(29),
  },
  [STATE(9)] = {
    [sym_paren_delimiter] = ACTIONS(33),
    [sym_paren_content] = ACTIONS(35),
  },
  [STATE(10)] = {
    FORTH_TOP_LEVEL_FOLLOW_ACTIONS(31),
  },
  [STATE(11)] = {
    [sym_paren_delimiter] = ACTIONS(39),
    [sym_paren_content] = ACTIONS(41),
  },
  [STATE(12)] = {
    FORTH_TOP_LEVEL_FOLLOW_ACTIONS(37),
  },
  [STATE(13)] = {
    [sym_paren_delimiter] = ACTIONS(45),
  },
  [STATE(14)] = {
    FORTH_TOP_LEVEL_FOLLOW_ACTIONS(43),
  },
  [STATE(15)] = {
    [sym_paren_delimiter] = ACTIONS(49),
  },
  [STATE(16)] = {
    FORTH_TOP_LEVEL_FOLLOW_ACTIONS(47),
  },
  [STATE(17)] = {
    FORTH_TOP_LEVEL_FOLLOW_ACTIONS(51),
  },
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 0, 0, 0),
  [5] = {.entry = {.count = 1, .reusable = true}}, SHIFT(3),
  [7] = {.entry = {.count = 1, .reusable = true}}, SHIFT(6),
  [9] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 1, 0, 0),
  [11] = {.entry = {.count = 1, .reusable = true}}, SHIFT(4),
  [13] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 1, 0, 0),
  [15] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2, 0, 0),
  [17] = {.entry = {.count = 1, .reusable = true}}, ACCEPT_INPUT(),
  [19] = {.entry = {.count = 1, .reusable = true}}, SHIFT(7),
  [21] = {.entry = {.count = 1, .reusable = true}}, SHIFT(8),
  [23] = {.entry = {.count = 1, .reusable = true}}, SHIFT(9),
  [25] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_paren_comment, 2, 0, 0),
  [27] = {.entry = {.count = 1, .reusable = true}}, SHIFT(10),
  [29] = {.entry = {.count = 1, .reusable = true}}, SHIFT(11),
  [31] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_paren_comment, 3, 0, 0),
  [33] = {.entry = {.count = 1, .reusable = true}}, SHIFT(12),
  [35] = {.entry = {.count = 1, .reusable = true}}, SHIFT(13),
  [37] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_stack_effect, 3, 0, 0),
  [39] = {.entry = {.count = 1, .reusable = true}}, SHIFT(14),
  [41] = {.entry = {.count = 1, .reusable = true}}, SHIFT(15),
  [43] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_stack_effect, 4, 0, 0),
  [45] = {.entry = {.count = 1, .reusable = true}}, SHIFT(16),
  [47] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_stack_effect, 4, 0, 0),
  [49] = {.entry = {.count = 1, .reusable = true}}, SHIFT(17),
  [51] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_stack_effect, 5, 0, 0),
};

#ifdef __cplusplus
extern "C" {
#endif
#ifdef TREE_SITTER_HIDE_SYMBOLS
#define TS_PUBLIC
#elif defined(_WIN32)
#define TS_PUBLIC __declspec(dllexport)
#else
#define TS_PUBLIC __attribute__((visibility("default")))
#endif

TS_PUBLIC const TSLanguage *tree_sitter_forth(void) {
  static const TSLanguage language = {
    .abi_version = LANGUAGE_VERSION,
    .symbol_count = SYMBOL_COUNT,
    .alias_count = ALIAS_COUNT,
    .token_count = TOKEN_COUNT,
    .external_token_count = EXTERNAL_TOKEN_COUNT,
    .state_count = STATE_COUNT,
    .large_state_count = LARGE_STATE_COUNT,
    .production_id_count = PRODUCTION_ID_COUNT,
    .supertype_count = SUPERTYPE_COUNT,
    .field_count = FIELD_COUNT,
    .max_alias_sequence_length = MAX_ALIAS_SEQUENCE_LENGTH,
    .parse_table = &ts_parse_table[0][0],
    .parse_actions = ts_parse_actions,
    .symbol_names = ts_symbol_names,
    .symbol_metadata = ts_symbol_metadata,
    .public_symbol_map = ts_symbol_map,
    .alias_sequences = &ts_alias_sequences[0][0],
    .lex_modes = ts_lex_modes,
    .lex_fn = ts_lex,
    .primary_state_ids = ts_primary_state_ids,
    .name = "forth",
    .max_reserved_word_set_size = 0,
    .metadata = {.major_version = 0, .minor_version = 4, .patch_version = 0},
  };
  return &language;
}
#ifdef __cplusplus
}
#endif
