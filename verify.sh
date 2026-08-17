#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"
python3 - <<'PY'
import json, re, tomllib
from pathlib import Path
json.load(open('snippets/forth.json', encoding='utf-8'))
json.load(open('tree-sitter.json', encoding='utf-8'))
tomllib.loads(Path('languages/forth/config.toml').read_text(encoding='utf-8'))
# Every named node used by queries must exist in the bundled grammar.
nodes = {x['type'] for x in json.load(open('src/node-types.json', encoding='utf-8'))}
for p in Path('languages/forth').glob('*.scm'):
    text = p.read_text(encoding='utf-8')
    for name in re.findall(r'\(([a-zA-Z_][a-zA-Z0-9_]*)\)', text):
        if name not in nodes:
            raise SystemExit(f'{p}: unknown grammar node {name}')
print('JSON/TOML/query node checks: OK')
PY
if command -v cc >/dev/null 2>&1; then
  cc -std=c11 -fsyntax-only -I src src/parser.c
  echo "Bundled Tree-sitter parser C check: OK"
fi
# User examples are deliberately included as regression fixtures.
count=$(find examples/openos -maxdepth 1 -type f -name '*.fs' | wc -l)
[ "$count" -eq 11 ]
echo "OpenOS regression examples: $count/11 present"
# Runtime dependency check: no remote repository references.
if grep -RInE 'github\.com|gitlab\.com|bitbucket\.org|AlexanderBrevig|https?://' . \
    --exclude-dir=.git --exclude='README.md' --exclude='verify.sh' >/dev/null; then
  echo "error: external repository/URL reference found" >&2
  grep -RInE 'github\.com|gitlab\.com|bitbucket\.org|AlexanderBrevig|https?://' . \
    --exclude-dir=.git --exclude='README.md' --exclude='verify.sh' >&2
  exit 1
fi
echo "External repository references: none"
