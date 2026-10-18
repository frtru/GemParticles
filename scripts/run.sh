#!/usr/bin/env bash
# Runs GemParticles from a preset build (see scripts/bootstrap.sh).
#
# Usage: scripts/run.sh [--nvidia] [--cfg Release|Debug|RelWithDebInfo] [--build|--rebuild] [app arguments...]
#   --nvidia   render on the discrete NVIDIA GPU through PRIME render offload
#              (without it, the default GPU is used: the integrated Intel one on this laptop)
#   --cfg      which build to run (default: Release)
#   --build    build before running: only what changed since the last build is recompiled
#   --rebuild  clean, then build everything before running
# App arguments: [project] [--config path], see `scripts/run.sh --help`
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

cfg=Release
build=
while [[ $# -gt 0 ]]; do
  case "$1" in
    --nvidia)  export __NV_PRIME_RENDER_OFFLOAD=1 __GLX_VENDOR_LIBRARY_NAME=nvidia; shift ;;
    --cfg)     cfg="$2"; shift 2 ;;
    --build)   build=incremental; shift ;;
    --rebuild) build=clean; shift ;;
    *)         break ;;
  esac
done

preset="conan-${cfg,,}"
if [[ -n "$build" ]]; then
  cd "$ROOT"
  # A build folder conan generated but CMake never configured
  [[ -f "build/$cfg/CMakeCache.txt" ]] || cmake --preset "$preset"
  if [[ "$build" == clean ]]; then
    cmake --build --preset "$preset" -j"$(nproc)" --clean-first
  else
    cmake --build --preset "$preset" -j"$(nproc)"
  fi
fi

exe="$ROOT/build/$cfg/bin/GemParticlesApp_$cfg"
if [[ ! -x "$exe" ]]; then
  echo "Missing $exe" >&2
  echo "Build it with: scripts/run.sh --cfg $cfg --build (after scripts/bootstrap.sh $cfg)" >&2
  exit 1
fi
exec "$exe" "$@"
