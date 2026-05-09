#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/../../.." && pwd)"
APP_DIR="$ROOT_DIR/platforms/android/app/src/main/jniLibs"
ANDROID_HOME_DIR="${ANDROID_HOME:-$HOME/Android/Sdk}"
ANDROID_NDK_VER="${ANDROID_NDK_VERSION:-}"
ANDROID_STAGE_ABIS="${ANDROID_STAGE_ABIS:-arm64-v8a x86_64}"
ANDROID_STAGE_STRIP="${ANDROID_STAGE_STRIP:-1}"
STRIP_TOOL=""

if [[ -z "$ANDROID_NDK_VER" ]] && [[ -d "$ANDROID_HOME_DIR/ndk" ]]; then
  ANDROID_NDK_VER="$(ls -1 "$ANDROID_HOME_DIR/ndk" | sort -V | tail -n 1)"
fi

if [[ -n "$ANDROID_NDK_VER" ]]; then
  TOOLCHAIN_BIN="$ANDROID_HOME_DIR/ndk/$ANDROID_NDK_VER/toolchains/llvm/prebuilt/linux-x86_64/bin"
  if [[ -x "$TOOLCHAIN_BIN/llvm-strip" ]]; then
    STRIP_TOOL="$TOOLCHAIN_BIN/llvm-strip"
  fi
fi

copy_libs() {
  local abi="$1"
  local src_dir="$2"
  local dst_dir="$APP_DIR/$abi"

  # Skip if the build directory for this ABI doesn't exist
  if [[ ! -d "$src_dir" ]]; then
    echo "Skipping ABI '$abi': source directory '$src_dir' not found"
    return 0
  fi

  # Skip if there are no .so files to stage
  local libs=("$src_dir"/*.so)
  if [[ ! -e "${libs[0]}" ]]; then
    echo "Skipping ABI '$abi': no .so files found in '$src_dir'"
    return 0
  fi

  mkdir -p "$dst_dir"
  for src in "${libs[@]}"; do
    local dst="$dst_dir/$(basename "$src")"
    cp -f "$src" "$dst"
    if [[ "$ANDROID_STAGE_STRIP" != "0" ]] && [[ -n "$STRIP_TOOL" ]]; then
      "$STRIP_TOOL" --strip-debug "$dst"
      echo "Staged and stripped $src -> $dst"
    else
      echo "Staged $src -> $dst (strip disabled or strip tool unavailable)"
    fi
  done
}

declare -A ABI_TO_DIR=(
  ["arm64-v8a"]="$ROOT_DIR/build/android-arm64"
  ["x86_64"]="$ROOT_DIR/build/android-x86_64"
)

# Drop any previously staged ABI directories to avoid stale APK contents.
rm -rf "$APP_DIR/x86" "$APP_DIR/arm64-v8a" "$APP_DIR/x86_64"

for abi in $ANDROID_STAGE_ABIS; do
  src_dir="${ABI_TO_DIR[$abi]:-}"
  if [[ -z "$src_dir" ]]; then
    echo "Unsupported ABI '$abi'. Supported ABIs: arm64-v8a x86_64" >&2
    exit 1
  fi
  copy_libs "$abi" "$src_dir"
done
