## Mailcore 2: Introduction ##

MailCore 2 provides a simple and asynchronous Objective-C API to work with the e-mail protocols **IMAP**, **POP** and **SMTP**. The API has been redesigned from the ground up.  It features:

- **POP**, **IMAP** and **SMTP** support
- **[RFC822](http://www.ietf.org/rfc/rfc0822.txt)** parser and generator
- **Asynchronous** APIs
- **HTML** rendering of messages
- **iOS** and **Mac** support

[![Build Status](https://travis-ci.org/MailCore/mailcore2.png?branch=master)](https://travis-ci.org/MailCore/mailcore2)


## Installation ##

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
        - Set 'Other Linker Flags' under Build Settings: `-lctemplate -letpan -licudata -licui18n -licuuc -lxml2 -lsasl2 -liconv -ltidy -lz` `-licucore -lc++ -stdlib=libc++ -ObjC -lcrypto -lssl`
        - Make sure to use LLVM C++ standard library.  In Build Settings, locate 'C++ Standard Library', and select `libc++`.
        - In Build Phases, add a Target Dependency of `static mailcore2 osx`.
5. **For iOS** - If you're targeting iOS, you have to link against MailCore 2 as a static library:
    * Add `libMailCore-ios.a`
    * Add `CFNetwork.framework`
	* Add `Security.framework`
    * Set 'Other Linker Flags': `-lctemplate-ios -letpan-ios -licudata -licui18n -licuuc -lxml2 -lsasl2 -liconv -ltidy -lz` `-licucore -lstdc++ -stdlib=libstdc++ -ObjC`
    * Make sure to use GNU C++ standard library.  In Build Settings, locate 'C++ Standard Library', and select `libstdc++`.
    * In Build Phases, add a Target Dependency of `static mailcore2 ios`.
6. Profit.

Here's a video that shows all the steps for iOS:
http://www.youtube.com/watch?v=9fAo6oBzlQI

## Basic IMAP Usage ##

Using MailCore 2 is just a little more complex conceptually than the original MailCore.  All fetch requests in MailCore 2 are made asynchronously through a queue.  What does this mean?  Well, let's  take a look at a simple example:

```objc
    MCOIMAPSession *session = [[MCOIMAPSession alloc] init];
    [session setHostname:@"imap.gmail.com"];
    [session setPort:993];
    [session setUsername:@"ADDRESS@gmail.com"];
    [session setPassword:@"123456"];
    [session setConnectionType:MCOConnectionTypeTLS];

    MCOIMAPMessagesRequestKind requestKind = MCOIMAPMessagesRequestKindHeaders;
    NSString *folder = @"INBOX";
    MCOIndexSet *uids = [MCOIndexSet indexSetWithRange:MCORangeMake(1, UINT64_MAX)];

    MCOIMAPFetchMessagesOperation *fetchOperation = [session fetchMessagesByUIDOperationWithFolder:folder requestKind:requestKind uids:uids];

    [fetchOperation start:^(NSError * error, NSArray * fetchedMessages, MCOIndexSet * vanishedMessages) {
        //We've finished downloading the messages!

        //Let's check if there was an error:
        if(error) {
            NSLog(@"Error downloading message headers:%@", error);
        }

        //And, let's print out the messages...
        NSLog(@"The post man delivereth:%@", fetchedMessages);
    }];
```

In this sample, we retrieved and printed a list of email headers from an IMAP server.  In order to execute the fetch, we request an asynchronous operation object from the `MCOIMAPSession` instance with our parameters (more on this later).  This operation object is able to initiate a connection to Gmail when we call the `start` method.  Now here's where things get a little tricky.  We call the `start` function with an Objective-C block, which is executed on the main thread when the fetch operation completes.  The actual fetching from IMAP is done on a **background thread**, leaving your UI and other processing **free to use the main thread**.

## Documentation ##

* [Class documentation](http://libmailcore.com/mailcore2/api/index.html)
* [Wiki](https://github.com/MailCore/mailcore2/wiki)
