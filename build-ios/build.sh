
chmod 777 ../scripts/get-ios.sh

rm -R ../ExternalsSrc/libetpan/include/libetpan
mkdir -p ../ExternalsSrc/libetpan/include/libetpan
find ../ExternalsSrc/libetpan/src -name "*.h" -type file -exec cp {} ../ExternalsSrc/libetpan/include/libetpan \;

cp ../ExternalsSrc/libetpan/build-mac/config-ios/config.h ../ExternalsSrc/libetpan/include
cp ../ExternalsSrc/libetpan/build-mac/config-ios/libetpan-config.h ../ExternalsSrc/libetpan/include/libetpan


rm -R build

xcodebuild -workspace mailcore-ios-fw.xcworkspace -scheme "mailcore ios" -configuration Release -quiet -arch armv7 -arch arm64 -derivedDataPath build
xcodebuild -workspace mailcore-ios-fw.xcworkspace -scheme "mailcore ios" -configuration Release -quiet -arch x86_64 -sdk iphonesimulator -derivedDataPath build
lipo -create build/Build/Products/Release-iphoneos/MailCore.framework/MailCore build/Build/Products/Release-iphonesimulator/MailCore.framework/MailCore -output build/Build/Products/Release-iphoneos/MailCore.framework/MailCore
cp -R build/Build/Products/Release-iphoneos/MailCore.framework build/MailCore.framework
