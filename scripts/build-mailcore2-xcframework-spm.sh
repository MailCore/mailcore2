#!/bin/sh

# Create XCFramework
cd "$(dirname "$0")"
./build-mailcore2-xcframework.sh

cd ../
MAILCORE_DIR="`pwd`"
BUILD_DIR="$MAILCORE_DIR/.build"
DATE="`date +"%Y-%m-%d"`"
FRAMEWORK_NAME="MailCore2.xcframework"
ARCHIVE_NAME="MailCore2-$DATE.xcframework.zip"

cd $BUILD_DIR

# Prepare for Distribution
zip -r -X "$ARCHIVE_NAME" "$FRAMEWORK_NAME"
CHECKSUM="`swift package compute-checksum "$ARCHIVE_NAME"`"

echo "XCFramework Zip exported to: $BUILD_DIR/$ARCHIVE_NAME"
echo "Checksum: $CHECKSUM"
