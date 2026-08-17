#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"

if ! command -v git >/dev/null 2>&1; then
  echo "error: git is required" >&2
  exit 1
fi
if ! command -v python3 >/dev/null 2>&1; then
  echo "error: python3 is required" >&2
  exit 1
fi

# The same directory is both the extension repository and the Tree-sitter
# grammar repository.  No network remote is configured or required.
if [ ! -d .git ]; then
  git init -q
  git config user.name "OpenOS Forth Zed"
  git config user.email "local@openos.invalid"
fi

# Remove any stale generated manifest before creating the source snapshot.
rm -f extension.toml
mkdir -p grammars

# Commit only when the grammar/extension sources changed. extension.toml is
# ignored because its rev points back to this source commit.
git add -A
if ! git rev-parse --verify HEAD >/dev/null 2>&1 || ! git diff --cached --quiet; then
  git commit -q -m "Prepare self-contained OpenOS Forth Zed extension"
fi

REV="$(git rev-parse HEAD)"
URI="$(python3 - "$ROOT" <<'PY'
from pathlib import Path
import sys
print(Path(sys.argv[1]).resolve().as_uri())
PY
)"

python3 - "$URI" "$REV" <<'PY'
from pathlib import Path
import sys
uri, rev = sys.argv[1], sys.argv[2]
template = Path("extension.toml.template").read_text(encoding="utf-8")
manifest = template.replace("__LOCAL_REPOSITORY__", uri).replace("__GRAMMAR_REV__", rev)
Path("extension.toml").write_text(manifest, encoding="utf-8")
PY

printf '%s\n' "Prepared self-contained Zed extension." \
  "Grammar repository: $URI" \
  "Grammar revision:   $REV" \
  "" \
  "Now in Zed run: zed: install dev extension" \
  "and select: $ROOT"
