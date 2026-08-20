// ANS Forth / OOS Forth grammar source.
//
// Deliberately flat: arbitrary Forth defining words can change compilation
// semantics, so the editor grammar models the stable lexical layer. The one
// structured form is a parenthesized stack effect, whose delimiters, parameter
// text and `--` separator are exposed separately for useful highlighting.
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

    // A `--` token selects the stack-effect alternative. The bundled parser
    // groups each side into one `paren_content` node; that keeps the AST small
    // while still allowing Zed to style argument text independently.
    paren_comment: $ => seq(
      alias("(", $.paren_delimiter),
      optional($.paren_content),
      alias(")", $.paren_delimiter),
    ),

    stack_effect: $ => seq(
      alias("(", $.paren_delimiter),
      optional($.paren_content),
      alias(token(prec(2, "--")), $.stack_effect_separator),
      optional($.paren_content),
      alias(")", $.paren_delimiter),
    ),

    // This source rule documents the public node shape. The bundled parser.c
    // uses a small contextual lexer so content may also contain whitespace and
    // nested notation such as `fib(n)` without splitting the visible node.
    paren_content: $ => repeat1($._paren_atom),
    _paren_atom: $ => token(prec(1, /[^\s()]+(\([^()\r\n]*\))*/)),

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
