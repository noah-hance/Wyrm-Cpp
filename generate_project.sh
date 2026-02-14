#!/usr/bin/env bash
set -euo pipefail

# -----------------------------------------------------------------
# generate_project.sh
# -----------------------------------------------------------------
# This script configures the project into ./build.
# Usage from repository root:
#   ./generate_project.sh
#
# It uses Ninja when available, otherwise Unix Makefiles.
# This keeps VS Code workflow consistent across platforms.
# -----------------------------------------------------------------

BUILD_DIR="build"

mkdir -p "${BUILD_DIR}"

if command -v ninja >/dev/null 2>&1; then
    GENERATOR="Ninja"
else
    GENERATOR="Unix Makefiles"
fi

echo "[1/2] Configuring with generator: ${GENERATOR}"
cmake -S . -B "${BUILD_DIR}" -G "${GENERATOR}"

echo "[2/2] Done."
echo "Build files are in ./${BUILD_DIR}"
