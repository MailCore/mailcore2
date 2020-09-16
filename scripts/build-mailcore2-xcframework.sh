#!/bin/sh

cd "$(dirname "$0")"
cd ../

MAILCORE_DIR="`pwd`"
BUILD_DIR="$MAILCORE_DIR/.build"
DATE="`date +"%Y-%m-%d"`"
FRAMEWORK_NAME="mailcore2-$DATE.xcframework.zip"

mkdir $BUILD_DIR

cd build-mac

# Build Mac Archive - ORIG
#xcodebuild archive -scheme "mailcore osx" \
#    -destination "platform=OS X,arch=x86_64" \
#    -archivePath "$BUILD_DIR/mailcore2.macOS.xcarchive" \
#    SKIP_INSTALL=NO \
#    BUILD_LIBRARY_FOR_DISTRIBUTION=YES

# Build Mac Archive
xcodebuild archive -scheme "mailcore osx" \
    -arch "x86_64" \
    -archivePath "$BUILD_DIR/mailcore2.macOS.xcarchive" \
    SKIP_INSTALL=NO \
    BUILD_LIBRARY_FOR_DISTRIBUTION=YES
    
# Build iOS Archive
xcodebuild archive -scheme "mailcore ios" \
    -destination "generic/platform=iOS" \
    -archivePath "$BUILD_DIR/mailcore2.iOS.xcarchive" \
    -sdk iphoneos \
    SKIP_INSTALL=NO \
    BUILD_LIBRARY_FOR_DISTRIBUTION=YES
    
# Build iOS Simulator Archive - ORIG
#xcodebuild archive -scheme "mailcore ios" \
#    -destination "platform=iOS Simulator,name=iPhone 11" \
#    -archivePath "$BUILD_DIR/mailcore2.iOS-Simulator.xcarchive" \
#    -sdk iphonesimulator \
#    SKIP_INSTALL=NO \
#    BUILD_LIBRARY_FOR_DISTRIBUTION=YES

# Build iOS Simulator Archive
xcodebuild archive -scheme "mailcore ios" \
    -arch "x86_64" \
    -archivePath "$BUILD_DIR/mailcore2.iOS-Simulator.xcarchive" \
    -sdk iphonesimulator \
    SKIP_INSTALL=NO \
    BUILD_LIBRARY_FOR_DISTRIBUTION=YES

# Build Mac Catalyst Archive - UNCOMMENT ONCE MAC CATALYST BUILDING IS FIXED
#xcodebuild archive -scheme "mailcore ios" \
#    -destination "platform=macOS,variant=Mac Catalyst" \
#    -archivePath "$BUILD_DIR/mailcore2.macOS-Catalyst.xcarchive" \
#    -sdk ???? \
#    SKIP_INSTALL=NO \
#    BUILD_LIBRARY_FOR_DISTRIBUTION=YES

cd $BUILD_DIR

# Create Combined XCArchive - REMOVE ONCE MAC CATALYST BUILDING IS FIXED
xcodebuild -create-xcframework \
	-framework "mailcore2.macOS.xcarchive/Products/Frameworks/MailCore.framework" \
	-framework "mailcore2.iOS-Simulator.xcarchive/Products/Frameworks/MailCore.framework" \
	-framework "mailcore2.iOS.xcarchive/Products/Frameworks/MailCore.framework" \
	-output "mailcore2.xcframework"

# Create Combine XCArchive - UNCOMMENT ONCE MAC CATALYST BUILDING IS FIXED
# xcodebuild -create-xcframework \
# 	-framework "$BUILD_DIR/mailcore2.macOS.xcarchive/Products/Frameworks/MailCore.framework" \
# 	-framework "$BUILD_DIR/mailcore2.iOS-Simulator.xcarchive/Products/Frameworks/MailCore.framework" \
# 	-framework "$BUILD_DIR/mailcore2.iOS.xcarchive/Products/Frameworks/MailCore.framework" \
# 	-framework "$BUILD_DIR/mailcore2.macOS-Catalyst.xcarchive/Products/Frameworks/MailCore.framework"
# 	-output "$BUILD_DIR/mailcore2.xcframework"

# Prepare for Distribution
zip -r -X "$FRAMEWORK_NAME" "mailcore2.xcframework"
CHECKSUM="`swift package compute-checksum "$BUILD_DIR/$FRAMEWORK_NAME"`"

# Clean Up
rm -rf "$BUILD_DIR/mailcore2.macOS.xcarchive"
rm -rf "$BUILD_DIR/mailcore2.iOS-Simulator.xcarchive"
rm -rf "$BUILD_DIR/mailcore2.iOS.xcarchive"
rm -rf "$BUILD_DIR/mailcore2.xcframework"

echo "XCFramework exported to: $BUILD_DIR/$FRAMEWORK_NAME"
echo "Checksum: $CHECKSUM"
