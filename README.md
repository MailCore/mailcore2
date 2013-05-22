## Mailcore 2

MailCore 2 brings a new API redesigned from ground.

- POP, IMAP and SMTP support
- RFC822 parser and generator
- asynchronous APIs
- HTML rendering of messages

## Quick project setup instruction

### Mac framework

- Add mailcore.framework
- Make sure to use LLVM C++ standard library.

### Mac static library

- Add libMailCore.a
- Set "Other Linker Flags": `-lctemplate -letpan -licudata -licui18n -licuuc -lxml2 -lsasl2 -liconv -ltidy -lc++ -all_load`
- Make sure to use LLVM C++ standard library.

### iOS static library

- Add libMailCore-ios.a
- Set "Other Linker Flags": `lctemplate-ios -letpan-ios -licudata -licui18n -licuuc -lxml2 -lsasl2 -liconv -ltidy -lstdc++ -all_load`
- Make sure to use GNU C++ standard library.
