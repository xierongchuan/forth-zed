# Forth / OpenOS Forth for Zed

A self-contained Zed language extension for ANS-style Forth plus the OpenOS Forth dialect.
The same repository is also the bundled Tree-sitter grammar repository. It has no runtime
or source dependency on a third-party grammar repository and contains its generated
`src/parser.c`.

## Install locally

After extracting the ZIP, run once from this directory:

```bash
./prepare.sh
```

Then open Zed and run `zed: install dev extension`, selecting this directory.

`prepare.sh` does not download anything. It creates a local Git commit, computes this
folder's `file://` URL and writes `extension.toml` with that local URL and exact local
revision. This small preparation step is necessary because Zed requires every grammar
entry to specify a Git repository and revision, even when the grammar lives locally.

## Included editor support

- `.fs`, `.fth`, `.4th`, `.frt`, `.blk` detection
- Tree-sitter highlighting and lexical structure
- `\` line comments, parenthesized comments and `( ... -- ... )` stack effects
- `: name ... ;` definition names in Outline
- indentation and structural `:` / `;` matching
- comment text objects
- ANS Forth words and operators
- snippet-based completion without an LSP
- OpenOS words such as `peek`, `poke`, `call`, `hex,`, `data-base`, `<=`, `>=`
- `$HEX`, `0xHEX`, `%binary`, `&octal`, decimal/float numbers
- `."..."`, `s"..."`, `c"..."`, `abort"..."` strings

## Regression examples

`examples/openos/` contains the 11 OpenOS Forth programs used while adapting the extension:
`fact`, `fibo`, `fizzbuzz`, `greet`, `hello`, `launch`, `mmio`, `multable`, `primes`,
`squares`, and `stars`.

## Verify the package

```bash
./verify.sh
```

The check validates JSON/TOML, query node names, the bundled C parser when a C compiler is
available, all 11 OpenOS examples, and that no external repository URL slipped into the
runtime source.

## Why the grammar is intentionally flat

Forth can extend its own dictionary and compilation semantics at run time. A parser that
tries to impose a C-like statement/expression AST quickly becomes wrong for user-defined
defining words. This grammar therefore parses the reliable lexical layer and lets Zed
queries classify control-flow words, stack operations, defining words, I/O words and
OpenOS-specific vocabulary. That keeps editing resilient even when OpenOS adds new words.
