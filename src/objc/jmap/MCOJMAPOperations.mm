//
//  MCOJMAPOperations.mm
//  mailcore2
//

#import "MCOJMAPOperations.h"

#include "MCAsyncJMAP.h"
#import "MCOOperation+Private.h"
#import "MCOUtils.h"

typedef void (^MailboxesCompletionType)(NSError * error, NSArray * mailboxes);
typedef void (^MessageIDsCompletionType)(NSError * error, NSArray<NSString *> * messageIDs);
typedef void (^MessagesCompletionType)(NSError * error, NSArray<MCOJMAPMessage *> * messages);
typedef void (^UploadCompletionType)(NSError * error, MCOJMAPBlobUpload * upload);
typedef void (^DownloadCompletionType)(NSError * error, NSData * data);
typedef void (^CreateDraftCompletionType)(NSError * error, MCOJMAPMessage * message);
typedef void (^SendCompletionType)(NSError * error, MCOJMAPSubmission * submission);

@implementation MCOJMAPFetchMailboxesOperation {
    MailboxesCompletionType _completionBlock;
}

#define nativeType mailcore::JMAPFetchMailboxesOperation

+ (void) load { MCORegisterClass(self, &typeid(nativeType)); }
+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object { return [[[self alloc] initWithMCOperation:(nativeType *) object] autorelease]; }
- (void) dealloc { [_completionBlock release]; [super dealloc]; }
- (void) start:(void (^)(NSError * error, NSArray * mailboxes))completionBlock { _completionBlock = [completionBlock copy]; [self start]; }
- (void) cancel { [_completionBlock release]; _completionBlock = nil; [super cancel]; }
- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;
    nativeType * op = MCO_NATIVE_INSTANCE;
    _completionBlock([NSError mco_errorWithErrorCode:op->error()], MCO_TO_OBJC(op->mailboxes()));
    [_completionBlock release];
    _completionBlock = nil;
}

@end

#undef nativeType

@implementation MCOJMAPQueryMessagesOperation {
    MessageIDsCompletionType _completionBlock;
}

#define nativeType mailcore::JMAPQueryMessagesOperation

+ (void) load { MCORegisterClass(self, &typeid(nativeType)); }
+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object { return [[[self alloc] initWithMCOperation:(nativeType *) object] autorelease]; }
- (void) dealloc { [_completionBlock release]; [super dealloc]; }
- (void) start:(void (^)(NSError * error, NSArray<NSString *> * messageIDs))completionBlock { _completionBlock = [completionBlock copy]; [self start]; }
- (void) cancel { [_completionBlock release]; _completionBlock = nil; [super cancel]; }
- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;
    nativeType * op = MCO_NATIVE_INSTANCE;
    _completionBlock([NSError mco_errorWithErrorCode:op->error()], MCO_TO_OBJC(op->messageIDs()));
    [_completionBlock release];
    _completionBlock = nil;
}

@end

#undef nativeType

@implementation MCOJMAPFetchMessagesOperation {
    MessagesCompletionType _completionBlock;
}

#define nativeType mailcore::JMAPFetchMessagesOperation

+ (void) load { MCORegisterClass(self, &typeid(nativeType)); }
+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object { return [[[self alloc] initWithMCOperation:(nativeType *) object] autorelease]; }
- (void) dealloc { [_completionBlock release]; [super dealloc]; }
- (void) start:(void (^)(NSError * error, NSArray<MCOJMAPMessage *> * messages))completionBlock { _completionBlock = [completionBlock copy]; [self start]; }
- (void) cancel { [_completionBlock release]; _completionBlock = nil; [super cancel]; }
- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;
    nativeType * op = MCO_NATIVE_INSTANCE;
    _completionBlock([NSError mco_errorWithErrorCode:op->error()], MCO_TO_OBJC(op->messages()));
    [_completionBlock release];
    _completionBlock = nil;
}

@end

#undef nativeType

@implementation MCOJMAPUploadOperation {
    UploadCompletionType _completionBlock;
}

#define nativeType mailcore::JMAPUploadOperation

+ (void) load { MCORegisterClass(self, &typeid(nativeType)); }
+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object { return [[[self alloc] initWithMCOperation:(nativeType *) object] autorelease]; }
- (void) dealloc { [_completionBlock release]; [super dealloc]; }
- (void) start:(void (^)(NSError * error, MCOJMAPBlobUpload * upload))completionBlock { _completionBlock = [completionBlock copy]; [self start]; }
- (void) cancel { [_completionBlock release]; _completionBlock = nil; [super cancel]; }
- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;
    nativeType * op = MCO_NATIVE_INSTANCE;
    _completionBlock([NSError mco_errorWithErrorCode:op->error()], MCO_TO_OBJC(op->upload()));
    [_completionBlock release];
    _completionBlock = nil;
}

@end

#undef nativeType

@implementation MCOJMAPDownloadOperation {
    DownloadCompletionType _completionBlock;
}

#define nativeType mailcore::JMAPDownloadOperation

+ (void) load { MCORegisterClass(self, &typeid(nativeType)); }
+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object { return [[[self alloc] initWithMCOperation:(nativeType *) object] autorelease]; }
- (void) dealloc { [_completionBlock release]; [super dealloc]; }
- (void) start:(void (^)(NSError * error, NSData * data))completionBlock { _completionBlock = [completionBlock copy]; [self start]; }
- (void) cancel { [_completionBlock release]; _completionBlock = nil; [super cancel]; }
- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;
    nativeType * op = MCO_NATIVE_INSTANCE;
    _completionBlock([NSError mco_errorWithErrorCode:op->error()], MCO_TO_OBJC(op->data()));
    [_completionBlock release];
    _completionBlock = nil;
}

@end

#undef nativeType

@implementation MCOJMAPCreateDraftOperation {
    CreateDraftCompletionType _completionBlock;
}

#define nativeType mailcore::JMAPCreateDraftOperation

+ (void) load { MCORegisterClass(self, &typeid(nativeType)); }
+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object { return [[[self alloc] initWithMCOperation:(nativeType *) object] autorelease]; }
- (void) dealloc { [_completionBlock release]; [super dealloc]; }
- (void) start:(void (^)(NSError * error, MCOJMAPMessage * message))completionBlock { _completionBlock = [completionBlock copy]; [self start]; }
- (void) cancel { [_completionBlock release]; _completionBlock = nil; [super cancel]; }
- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;
    nativeType * op = MCO_NATIVE_INSTANCE;
    _completionBlock([NSError mco_errorWithErrorCode:op->error()], MCO_TO_OBJC(op->message()));
    [_completionBlock release];
    _completionBlock = nil;
}

@end

#undef nativeType

@implementation MCOJMAPSendOperation {
    SendCompletionType _completionBlock;
}

#define nativeType mailcore::JMAPSendOperation

+ (void) load { MCORegisterClass(self, &typeid(nativeType)); }
+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object { return [[[self alloc] initWithMCOperation:(nativeType *) object] autorelease]; }
- (void) dealloc { [_completionBlock release]; [super dealloc]; }
- (void) start:(void (^)(NSError * error, MCOJMAPSubmission * submission))completionBlock { _completionBlock = [completionBlock copy]; [self start]; }
- (void) cancel { [_completionBlock release]; _completionBlock = nil; [super cancel]; }
- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;
    nativeType * op = MCO_NATIVE_INSTANCE;
    _completionBlock([NSError mco_errorWithErrorCode:op->error()], MCO_TO_OBJC(op->submission()));
    [_completionBlock release];
    _completionBlock = nil;
}

@end

#undef nativeType
