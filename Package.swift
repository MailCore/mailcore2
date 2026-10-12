// swift-tools-version:5.3
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "MailCore2",
    platforms: [
        .iOS(.v9), .macOS(.v10_10)
    ],
    products: [
        .library(
            name: "MailCore2",
            targets: ["MailCore2"]),
    ],
    targets: [
        .binaryTarget(name: "MailCore2",
                      url: "https://github.com/MailCore/mailcore2/raw/master/bin/MailCore2-2020-09-24.xcframework.zip",
                      checksum: "c3479968c758094165fb0b4de5ca7dd9f8aafac423388c51406c447f69a1b853")
    ]
)
