# Froth / OOS Forth for Zed

A self-contained Zed language extension for ANS-style Forth and OOS Forth, the
Forth dialect used by OpenOS. It provides syntax-aware highlighting, navigation,
indentation and snippets without requiring an LSP or an external grammar at run
time.

The extension repository also contains its Tree-sitter parser, so local installs
are reproducible and do not depend on a third-party grammar repository.

## Install locally

After extracting the repository, run once from this directory:

```bash
./prepare.sh
```

Then open Zed and run `zed: install dev extension`, selecting this directory.

`prepare.sh` does not download anything. It creates a local Git snapshot, computes
this folder's `file://` URL and writes `extension.toml` with that local URL and exact
revision. Zed requires a repository and revision for grammar entries even when the
grammar lives in the extension itself.

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
- snippet-based completion without an LSP, including OOS-specific argument/process/file words
- OOS Forth runtime words, including `depth`, `getpid`, `argc`, `arg`,
  `s>number`, `load-file`, `save-file`, `peek`, `poke`, `call`, `hex,`,
  `data-base`, `ms`, `bye`, `<=`, and `>=`
- `$HEX`, `0xHEX`, `%binary`, `&octal`, decimal and floating-point numbers
- `."..."`, `s"..."`, `c"..."`, `abort"..."` strings

## OOS Forth source of truth

OOS-specific vocabulary and regression examples are synchronized against the
OpenOS `experimental-heap-bc` branch rather than guessed from generic Forth word
lists. Generic ANS-style highlighting remains available for ordinary Forth files.

## Regression examples

`examples/openos/` contains the OpenOS Forth programs used while adapting and
regression-testing the extension: `counter`, `fact`, `fibo`, `fizzbuzz`, `greet`,
`hello`, `launch`, `mmio`, `multable`, `primes`, `squares`, and `stars`.

`counter.fs` also covers the OOS process/argument words `getpid`, `argc`, `arg`,
`s>number`, and `ms`.

## Verify the package

```bash
./verify.sh
```

The check validates JSON/TOML, query node names, the bundled C parser when a C
compiler is available, all 12 OOS regression examples, and that no external
repository URL slipped into runtime source.

If the `tree-sitter` CLI is installed, the verification script also runs the corpus
tests.

## Why the grammar is intentionally mostly flat

Forth can extend its own dictionary and compilation semantics at run time. A parser
that tries to impose a C-like statement/expression AST quickly becomes wrong for
user-defined defining words. The grammar therefore parses the reliable lexical
layer and lets Zed queries classify control flow, stack operations, defining words,
I/O words and OOS-specific vocabulary.

Stack effects are the deliberate exception. They are stable documentation syntax,
so `( ... -- ... )` is split into delimiters, parameter text and the separator. That
gives parameters their own visual role without pretending that stack-effect names
are run-time variables.
