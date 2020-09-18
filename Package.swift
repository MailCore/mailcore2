// swift-tools-version:5.3
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "MailCore2",
    platforms: [
        .iOS(.v8), .macOS(.v10_10)
    ],
    products: [
//        .library(
//            name: "MailCore2",
//            targets: ["MailCore2"]),
        .library(name: "MailCore2", type: .static, targets: ["MailCore2"])
    ],
    targets: [
        .binaryTarget(name: "MailCore2",
                      url: "https://downloads.maddux.cloud/mailcore2-apple-xcframework/MailCore2-latest.xcframework.zip",
                      checksum: "9ac3688a216e9c38dac7f5ddbfa89d314674da7f7f390d366774d218e9955452")
    ]
)
