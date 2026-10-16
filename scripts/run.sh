#!/usr/bin/env bash
# Runs GemParticles from a preset build (see scripts/bootstrap.sh).
#
# Usage: scripts/run.sh [--nvidia] [--cfg Release|Debug|RelWithDebInfo] [app arguments...]
#   --nvidia  render on the discrete NVIDIA GPU through PRIME render offload
#             (without it, the default GPU is used: the integrated Intel one on this laptop)
#   --cfg     which build to run (default: Release)
# App arguments: see `scripts/run.sh --help`
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

cfg=Release
while [[ $# -gt 0 ]]; do
  case "$1" in
    --nvidia) export __NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia; shift ;;
    --cfg)    cfg="$2"; shift 2 ;;
    *)        break ;;
  esac
done

exe="$ROOT/build/$cfg/bin/GemParticlesApp_$cfg"
if [[ ! -x "$exe" ]]; then
  echo "Missing $exe" >&2
  echo "Build it with: cmake --preset conan-${cfg,,} && cmake --build --preset conan-${cfg,,}" >&2
  exit 1
fi
exec "$exe" "$@"
