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
                      url: "http://64.227.19.239/mailcore2-apple-xcframework/mailcore2-latest.xcframework.zip",
                      checksum: "847b486bf00cd5eeb812e3297f963d9281bc121bdd8278837614dcc46895c0ea")
    ]
)
