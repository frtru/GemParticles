#!/usr/bin/env bash
# Runs the tests of a preset build (see scripts/bootstrap.sh) with ctest.
#
# Usage: scripts/test.sh [--nvidia] [--cfg Release|Debug|RelWithDebInfo] [ctest arguments...]
#   --nvidia  run the system tests' app on the discrete NVIDIA GPU (PRIME render offload)
#   --cfg     which build to test (default: Release)
# ctest arguments, e.g.: -L unit (no window needed), -L system (needs a display), -R Config
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

build="$ROOT/build/$cfg"
if [[ ! -f "$build/CTestTestfile.cmake" ]]; then
  echo "Missing tests in $build" >&2
  echo "Build them with: cmake --preset conan-${cfg,,} && cmake --build --preset conan-${cfg,,}" >&2
  exit 1
fi
exec ctest --test-dir "$build" --output-on-failure "$@"
