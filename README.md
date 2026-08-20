# OSS / ANS Forth for Zed

A self-contained Zed language extension for ANS-style Forth and OOS Forth, the Forth version used by OpenOS.

The extension provides syntax highlighting, outline support, indentation, comments, text objects and snippets without requiring an LSP.

## Install locally

Run once from this directory:

```bash
./prepare.sh
```

Then open Zed and run `zed: install dev extension`, selecting this directory.

## Included editor support

- `.fs`, `.fth`, `.4th`, `.frt`, `.blk` detection
- Tree-sitter highlighting and lexical structure
- `\` line comments and parenthesized `( ... )` comments
- structured stack effects such as `( addr len -- buf n )`
- stack-effect parameter highlighting distinct from comment text
- `: name ... ;` definitions in Outline
- variables, constants and other defining words in Outline
- indentation and structural `:` / `;` matching
- comment text objects
- ANS-style Forth words, operators and control flow
- snippet-based completion without an LSP
- OOS Forth words including `depth`, `getpid`, `argc`, `arg`, `s>number`, `load-file`, `save-file`, `peek`, `poke`, `call`, `hex,`, `data-base`, `ms`, `bye`, `<=`, and `>=`
- `$HEX`, `0xHEX`, `%binary`, `&octal`, decimal and floating-point numbers
- `."..."`, `s"..."`, `c"..."`, `abort"..."` strings

## Regression examples

`examples/openos/` contains small OOS Forth examples used to verify highlighting and parsing of definitions, variables, stack effects, control flow, strings, arithmetic, file/process words and MMIO-style words.

## Verify the package

```bash
./verify.sh
```

The check validates JSON/TOML files, Tree-sitter query node names, the bundled parser, regression examples and the absence of accidental external runtime dependencies.

If the `tree-sitter` CLI is installed, the verification script also runs the corpus tests.

## Grammar design

Forth can extend its dictionary and compilation semantics at run time, so the grammar keeps the general language structure intentionally lightweight.

Stack effects are parsed more precisely because they have stable documentation syntax. Expressions such as `( addr len -- buf n )` expose delimiters, parameter text and the `--` separator separately, allowing Zed to style stack-effect parameters independently from ordinary comments.
