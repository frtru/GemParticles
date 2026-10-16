#!/usr/bin/env bash
# Installs the conan dependencies and generates a CMake preset per build type.
# Dependencies are always built in Release; only GemParticles itself changes build type.
#
# Usage: scripts/bootstrap.sh [Release|Debug|RelWithDebInfo ...]   (default: all three)
# Then:  cmake --preset conan-<type> && cmake --build --preset conan-<type>   (type in lowercase)
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

CONAN="$(command -v conan || echo "$ROOT/.venv/bin/conan")"
if [[ ! -x "$CONAN" ]]; then
  echo "Installing conan into .venv ..."
  python3 -m venv .venv
  .venv/bin/pip install --quiet conan
fi
"$CONAN" profile detect --exist-ok

build_types=("$@")
[[ ${#build_types[@]} -eq 0 ]] && build_types=(Release Debug RelWithDebInfo)
for build_type in "${build_types[@]}"; do
  "$CONAN" install . -s build_type=Release -s "&:build_type=$build_type" --build=missing
done
