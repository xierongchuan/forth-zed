#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"

python3 - <<'PY'
import json, re, tomllib
from pathlib import Path

config = tomllib.loads(Path('languages/forth/config.toml').read_text(encoding='utf-8'))
manifest = tomllib.loads(Path('extension.toml.template').read_text(encoding='utf-8'))
tree_sitter = json.load(open('tree-sitter.json', encoding='utf-8'))
for snippet_file in Path('snippets').glob('*.json'):
    json.load(open(snippet_file, encoding='utf-8'))

assert config['name'] == 'Froth / OOS Forth'
assert manifest['name'] == 'Froth / OOS Forth'
assert manifest['version'] == '0.2.0'
assert './snippets/oos.json' in manifest['snippets']
assert tree_sitter['metadata']['version'] == '0.4.0'

nodes = {x['type'] for x in json.load(open('src/node-types.json', encoding='utf-8'))}
for p in Path('languages/forth').glob('*.scm'):
    text = p.read_text(encoding='utf-8')
    for name in re.findall(r'\(([a-zA-Z_][a-zA-Z0-9_]*)\)', text):
        if name not in nodes:
            raise SystemExit(f'{p}: unknown grammar node {name}')

required_stack_nodes = {'stack_effect', 'paren_delimiter', 'paren_content', 'stack_effect_separator'}
missing = required_stack_nodes - nodes
if missing:
    raise SystemExit(f'missing structured stack-effect nodes: {sorted(missing)}')

print('JSON/TOML/query node checks: OK')
PY

if command -v cc >/dev/null 2>&1; then
  cc -std=c11 -Wall -Wextra -Werror -fsyntax-only -I src src/parser.c
  echo "Bundled Tree-sitter parser C check: OK"
fi

if command -v tree-sitter >/dev/null 2>&1; then
  tree-sitter test
  echo "Tree-sitter corpus: OK"
fi

count=$(find examples/openos -maxdepth 1 -type f -name '*.fs' | wc -l)
[ "$count" -eq 12 ]
echo "OOS regression examples: $count/12 present"

if grep -RInE 'github\.com|gitlab\.com|bitbucket\.org|AlexanderBrevig|https?://' . \
    --exclude-dir=.git --exclude='README.md' --exclude='verify.sh' >/dev/null; then
  echo "error: external repository/URL reference found" >&2
  grep -RInE 'github\.com|gitlab\.com|bitbucket\.org|AlexanderBrevig|https?://' . \
    --exclude-dir=.git --exclude='README.md' --exclude='verify.sh' >&2
  exit 1
fi
echo "External repository references: none"
