#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT"

if [ ! -f extension.toml ]; then
  echo "error: extension.toml is missing" >&2
  exit 1
fi

printf '%s\n' \
  "This extension is already prepared for Zed." \
  "The publishable extension.toml is tracked in Git." \
  "" \
  "Now in Zed run: zed: install dev extension" \
  "and select: $ROOT"
