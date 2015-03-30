### Carthage ###

MailCore 2 is available through [Carthage](https://github.com/Carthage/Carthage).

Add this line to the `Cartfile`
```
github "MailCore/mailcore2"
```

Here you can find [Instructions for install Carthage](https://github.com/Carthage/Carthage#installing-carthage).

[![Carthage compatible](https://img.shields.io/badge/Carthage-compatible-4BC51D.svg?style=flat)](https://github.com/Carthage/Carthage)

### Cocoapods ###

MailCore 2 is available on [Cocoapods](http://cocoapods.org/).

**For iOS:**
```
pod 'mailcore2-ios'
```

**For OS X:**
```
pod 'mailcore2-osx'
```

### Binary ###

**For iOS:**
Download the latest [build for iOS](http://d.etpan.org/mailcore2-deps/mailcore2-ios/).

**For OS X:**
Download the latest [build for OS X](http://d.etpan.org/mailcore2-deps/mailcore2-osx/).

### Build for iOS/OSX ###

1. If you're migrating from MailCore1, you should first clean your build folder.
2. Checkout MailCore2 into a directory relative to your project.
3. Under the `build-mac` directory, locate the `mailcore2.xcodeproj` file, and drag this into your Xcode project.
4. **For Mac** - If you're building for Mac, you can either link against MailCore 2 as a framework, or as a static library:
    * Mac framework
        - Go to Build Phases from your build target, and under 'Link Binary With Libraries', add `MailCore.framework` and `Security.framework`.
        - Make sure to use LLVM C++ standard library.  Open Build Settings, scroll down to 'C++ Standard Library', and select `libc++`.
        - In Build Phases, add a Target Dependency of `mailcore osx` (it's the one with a little toolbox icon).
        - Goto `Editor > Add Build Phase > Copy Files`.
        - Expand the newly created Build Phase and change it's destination to "Frameworks".
        - Click the `+` icon and select `MailCore.framework`.
    * Mac static library
        - Go to Build Phases from your build target, and under 'Link Binary With Libraries', add `libMailCore.a` and `Security.framework`.
        - Set 'Other Linker Flags' under Build Settings: `-lctemplate -letpan -lxml2 -lsasl2 -liconv -ltidy -lz` `-lc++ -stdlib=libc++ -ObjC -lcrypto -lssl -lresolv`
        - Make sure to use LLVM C++ standard library.  In Build Settings, locate 'C++ Standard Library', and select `libc++`.
        - In Build Phases, add a Target Dependency of `static mailcore2 osx`.
5. **For iOS** - If you're targeting iOS, you have to link against MailCore 2 as a static library:
    * Add `libMailCore-ios.a`
    * Add `CFNetwork.framework`
	* Add `Security.framework`
    * Set 'Other Linker Flags': `-lctemplate-ios -letpan-ios -lxml2 -lsasl2 -liconv -ltidy -lz` `-lc++  -lresolv -stdlib=libc++ -ObjC`
    * Make sure to use LLVM C++ standard library.  Open Build Settings, scroll down to 'C++ Standard Library', and select `libc++`.
    * In Build Phases, add a Target Dependency of `static mailcore2 ios`.
6. **For Swift** - If you are using Mailcore in a Swift project you also need to complete the following steps:
    * Create a new header file in your project and name it ```Project-Name-Bridging-Header.h```.
    * Remove any template code from the file and add ```#import <MailCore/MailCore.h>```
    * In your target settings search for _Objective-c Bridging Header_ and add a link to your bridging header.
    * You do not need to import Mailcore in any of your classes as the bridging header takes care of this automatically.
7. Profit.

Here's a video that shows all the steps for iOS:
http://www.youtube.com/watch?v=9fAo6oBzlQI
