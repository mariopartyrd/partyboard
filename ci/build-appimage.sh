#!/bin/bash -ex
shopt -s extglob

# Detect architecture names used by linuxdeploy and distro library paths
ARCH="$(uname -m)"

case "$ARCH" in
    x86_64)
        LINUXDEPLOY_ARCH="x86_64"
        LIBUSB_PATH="/usr/lib/x86_64-linux-gnu/libusb-1.0.so"
        ;;
    aarch64|arm64)
        LINUXDEPLOY_ARCH="aarch64"
        LIBUSB_PATH="/usr/lib/aarch64-linux-gnu/libusb-1.0.so"
        ;;
    *)
        echo "Unsupported architecture: $ARCH"
        exit 1
        ;;
esac

# Get linuxdeploy
cd "$RUNNER_WORKSPACE"

curl -fOL \
  "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-${LINUXDEPLOY_ARCH}.AppImage"

chmod +x "linuxdeploy-${LINUXDEPLOY_ARCH}.AppImage"

# Build AppImage
cd "$GITHUB_WORKSPACE"

mkdir -p build/appdir/usr/{bin,lib,share/{applications,icons/hicolor}}

cp build/install/partyboard build/appdir/usr/bin/
cp -r build/install/res build/appdir/usr/bin/
cp build/install/libdol.so build/appdir/usr/bin/
cp build/install/*.so build/appdir/usr/lib/
cp -r platforms/freedesktop/{16x16,32x32,48x48,64x64,128x128,256x256,512x512,1024x1024} build/appdir/usr/share/icons/hicolor
cp platforms/freedesktop/partyboard.desktop build/appdir/usr/share/applications

cd build/install

VERSION="$PARTY_BOARD_VERSION" \
NO_STRIP=1 \
"$RUNNER_WORKSPACE"/"linuxdeploy-${LINUXDEPLOY_ARCH}.AppImage" \
  -l "$LIBUSB_PATH" \
  --appdir "$GITHUB_WORKSPACE"/build/appdir \
  --output appimage
