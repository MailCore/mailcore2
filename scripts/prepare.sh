#!/bin/sh

# If this script is run outside of Xcode, prepare both platforms by default
if [ -z "$SDKROOT" ]; then
    SDKROOT="MacOSX iPhone"
fi

if echo "$SDKROOT" | grep -q "MacOSX"; then
    echo "Preparing Mac"
    ./prepare-mac.sh
fi
if echo "$SDKROOT" | grep -q "iPhone"; then
    echo "Preparing iOS"
    ./prepare-ios.sh
fi
