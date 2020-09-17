// swift-tools-version:5.3
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "mailcore2",
    platforms: [
        .iOS(.v8), .macOS(.v10_10)
    ],
    products: [
        .library(
            name: "MailCore2",
            targets: ["MailCore2"]),
    ],
    targets: [
        .binaryTarget(name: "MailCore2",
                      url: "https://downloads.maddux.cloud/mailcore2-apple-xcframework/mailcore2-latest.xcframework.zip",
                      checksum: "73ad593ac65fa6717e914f3f53302bd9064ef027e168676ae52f8295a6057047")
    ]
)
