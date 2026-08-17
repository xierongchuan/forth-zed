// OpenOS/ANS Forth grammar source.
//
// Deliberately flat: arbitrary Forth defining words can change compilation
// semantics, so the editor grammar models the stable lexical layer. Semantic
// classes (control flow, stack ops, OpenOS words, etc.) live in Zed queries.
module.exports = grammar({
  name: "forth",

  extras: $ => [/\s+/],

  rules: {
    source_file: $ => repeat(choice(
      $.line_comment,
      $.paren_comment,
      $.stack_effect,
      $.string,
      $.number,
      $.character_literal,
      $.colon,
      $.semicolon,
      $.word,
    )),

    line_comment: $ => token(/\\[^\r\n]*/),
    paren_comment: $ => token(/\([^)]*\)/),
    stack_effect: $ => token(/\([^)]*--[^)]*\)/),

    string: $ => token(choice(
      /\."[^\"]*"/,
      /[sScC]"[^\"]*"/,
      /[aA][bB][oO][rR][tT]"[^\"]*"/,
    )),

    number: $ => token(choice(
      /[+-]?\$[0-9a-fA-F]+/,
      /[+-]?0[xX][0-9a-fA-F]+/,
      /[+-]?%[01]+/,
      /[+-]?&[0-7]+/,
      /[+-]?[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?/,
      /[+-]?[0-9]+([eE][+-]?[0-9]+)?/,
    )),

    character_literal: $ => token(/'[^\s]'/),
    colon: $ => token(":"),
    semicolon: $ => token(";"),
    word: $ => token(/[^\s]+/),
  },
});
