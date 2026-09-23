#import "MCOActiveSyncOperation.h"
#import "MCOActiveSyncOperation+Private.h"

#include "MCAsyncActiveSync.h"

#import "MCOOperation+Private.h"
#import "MCOUtils.h"

typedef void (^MCOActiveSyncCompletion)(NSError *error);

@implementation MCOActiveSyncOperation {
    MCOActiveSyncCompletion _completionBlock;
    MCOActiveSyncSession * _session;
}

#define nativeType mailcore::ActiveSyncOperation

- (void) dealloc
{
    [_session release];
    [_completionBlock release];
    [super dealloc];
}

- (void) start:(void (^)(NSError *error))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;

    NSError * error = [NSError mco_errorWithErrorCode:MCO_NATIVE_INSTANCE->error()];
    _completionBlock(error);
    [_completionBlock release];
    _completionBlock = nil;
}

- (void) setSession:(MCOActiveSyncSession *)session
{
    [_session release];
    _session = [session retain];
}

- (MCOActiveSyncSession *) session
{
    return _session;
}

@end

@implementation MCOActiveSyncOptionsOperation {
    void (^_completionBlock)(NSError *error, MCOActiveSyncOptions *options);
}

+ (void) load
{
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncOptionsOperation));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCOperation:(mailcore::Operation *) object] autorelease];
}

- (void) dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void) start:(void (^)(NSError *error, MCOActiveSyncOptions *options))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;

    mailcore::ActiveSyncOptionsOperation * op = (mailcore::ActiveSyncOptionsOperation *) [self mco_mcObject];
    if (op->error() == mailcore::ErrorNone) {
        _completionBlock(nil, MCO_TO_OBJC(op->options()));
    }
    else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

@end

@implementation MCOActiveSyncFolderSyncOperation {
    void (^_completionBlock)(NSError *error, MCOActiveSyncFolderSyncResult *result);
}

+ (void) load
{
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncFolderSyncOperation));
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncFolderResyncOperation));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCOperation:(mailcore::Operation *) object] autorelease];
}

- (void) dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void) start:(void (^)(NSError *error, MCOActiveSyncFolderSyncResult *result))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;

    mailcore::ActiveSyncOperation * op = MCO_NATIVE_INSTANCE;
    mailcore::ActiveSyncFolderSyncResult * result = NULL;
    if (mailcore::ActiveSyncFolderSyncOperation * folderSyncOp = dynamic_cast<mailcore::ActiveSyncFolderSyncOperation *>(op)) {
        result = folderSyncOp->result();
    }
    else if (mailcore::ActiveSyncFolderResyncOperation * folderResyncOp = dynamic_cast<mailcore::ActiveSyncFolderResyncOperation *>(op)) {
        result = folderResyncOp->result();
    }

    if (op->error() == mailcore::ErrorNone) {
        _completionBlock(nil, MCO_TO_OBJC(result));
    }
    else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

@end

@implementation MCOActiveSyncFolderMutationOperation {
    void (^_completionBlock)(NSError *error, MCOActiveSyncFolderMutationResult *result);
}

+ (void) load
{
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncFolderCreateOperation));
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncFolderUpdateOperation));
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncFolderDeleteOperation));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCOperation:(mailcore::Operation *) object] autorelease];
}

- (void) dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void) start:(void (^)(NSError *error, MCOActiveSyncFolderMutationResult *result))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;

    mailcore::ActiveSyncOperation * op = MCO_NATIVE_INSTANCE;
    mailcore::ActiveSyncFolderMutationResult * result = NULL;
    if (mailcore::ActiveSyncFolderCreateOperation * createOp = dynamic_cast<mailcore::ActiveSyncFolderCreateOperation *>(op)) {
        result = createOp->result();
    }
    else if (mailcore::ActiveSyncFolderUpdateOperation * updateOp = dynamic_cast<mailcore::ActiveSyncFolderUpdateOperation *>(op)) {
        result = updateOp->result();
    }
    else if (mailcore::ActiveSyncFolderDeleteOperation * deleteOp = dynamic_cast<mailcore::ActiveSyncFolderDeleteOperation *>(op)) {
        result = deleteOp->result();
    }

    if (op->error() == mailcore::ErrorNone) {
        _completionBlock(nil, MCO_TO_OBJC(result));
    }
    else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

@end

@implementation MCOActiveSyncSyncOperation {
    void (^_completionBlock)(NSError *error, MCOActiveSyncSyncResult *result);
}

+ (void) load
{
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncSyncOperation));
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncSyncMessagesOperation));
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncMarkMessagesReadOperation));
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncSetMessagesFlaggedOperation));
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncDeleteMessagesOperation));
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncMarkMessageReadOperation));
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncSetMessageFlaggedOperation));
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncDeleteMessageOperation));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCOperation:(mailcore::Operation *) object] autorelease];
}

- (void) dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void) start:(void (^)(NSError *error, MCOActiveSyncSyncResult *result))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;

    mailcore::ActiveSyncOperation * op = MCO_NATIVE_INSTANCE;
    mailcore::ActiveSyncSyncResult * result = NULL;
    if (mailcore::ActiveSyncSyncOperation * syncOp = dynamic_cast<mailcore::ActiveSyncSyncOperation *>(op)) {
        result = syncOp->result();
    }
    else if (mailcore::ActiveSyncSyncMessagesOperation * syncMessagesOp = dynamic_cast<mailcore::ActiveSyncSyncMessagesOperation *>(op)) {
        result = syncMessagesOp->result();
    }
    else if (mailcore::ActiveSyncMessageMutationOperation * mutationOp = dynamic_cast<mailcore::ActiveSyncMessageMutationOperation *>(op)) {
        result = mutationOp->result();
    }

    if (op->error() == mailcore::ErrorNone) {
        _completionBlock(nil, MCO_TO_OBJC(result));
    }
    else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

@end

@implementation MCOActiveSyncProvisionOperation {
    void (^_completionBlock)(NSError *error, MCOActiveSyncProvisionResult *result);
}

+ (void) load
{
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncProvisionOperation));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCOperation:(mailcore::Operation *) object] autorelease];
}

- (void) dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void) start:(void (^)(NSError *error, MCOActiveSyncProvisionResult *result))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;

    mailcore::ActiveSyncProvisionOperation * op = (mailcore::ActiveSyncProvisionOperation *) [self mco_mcObject];
    if (op->error() == mailcore::ErrorNone) {
        _completionBlock(nil, MCO_TO_OBJC(op->result()));
    }
    else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

@end

@implementation MCOActiveSyncItemEstimateOperation {
    void (^_completionBlock)(NSError *error, MCOActiveSyncItemEstimateResult *result);
}

+ (void) load
{
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncItemEstimateOperation));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCOperation:(mailcore::Operation *) object] autorelease];
}

- (void) dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void) start:(void (^)(NSError *error, MCOActiveSyncItemEstimateResult *result))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;

    mailcore::ActiveSyncItemEstimateOperation * op = (mailcore::ActiveSyncItemEstimateOperation *) [self mco_mcObject];
    if (op->error() == mailcore::ErrorNone) {
        _completionBlock(nil, MCO_TO_OBJC(op->result()));
    }
    else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

@end

@implementation MCOActiveSyncFetchMessageOperation {
    void (^_completionBlock)(NSError *error, MCOActiveSyncMessage *message);
}

+ (void) load
{
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncFetchMessageOperation));
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncFetchMessageBodyPartOperation));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCOperation:(mailcore::Operation *) object] autorelease];
}

- (void) dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void) start:(void (^)(NSError *error, MCOActiveSyncMessage *message))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;

    mailcore::ActiveSyncOperation * op = MCO_NATIVE_INSTANCE;
    mailcore::ActiveSyncMessage * message = NULL;
    if (mailcore::ActiveSyncFetchMessageOperation * fetchOp = dynamic_cast<mailcore::ActiveSyncFetchMessageOperation *>(op)) {
        message = fetchOp->message();
    }
    else if (mailcore::ActiveSyncFetchMessageBodyPartOperation * bodyPartOp = dynamic_cast<mailcore::ActiveSyncFetchMessageBodyPartOperation *>(op)) {
        message = bodyPartOp->message();
    }

    if (op->error() == mailcore::ErrorNone) {
        _completionBlock(nil, MCO_TO_OBJC(message));
    }
    else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

@end

@implementation MCOActiveSyncFetchAttachmentOperation {
    void (^_completionBlock)(NSError *error, MCOActiveSyncAttachmentData *data);
}

+ (void) load
{
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncFetchAttachmentOperation));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCOperation:(mailcore::Operation *) object] autorelease];
}

- (void) dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void) start:(void (^)(NSError *error, MCOActiveSyncAttachmentData *data))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;

    mailcore::ActiveSyncFetchAttachmentOperation * op = (mailcore::ActiveSyncFetchAttachmentOperation *) [self mco_mcObject];
    if (op->error() == mailcore::ErrorNone) {
        _completionBlock(nil, MCO_TO_OBJC(op->attachmentData()));
    }
    else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

@end

@implementation MCOActiveSyncMoveOperation {
    void (^_completionBlock)(NSError *error, MCOActiveSyncMoveResult *result);
}

+ (void) load
{
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncMoveMessagesOperation));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCOperation:(mailcore::Operation *) object] autorelease];
}

- (void) dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void) start:(void (^)(NSError *error, MCOActiveSyncMoveResult *result))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;

    mailcore::ActiveSyncMoveMessagesOperation * op = (mailcore::ActiveSyncMoveMessagesOperation *) [self mco_mcObject];
    if (op->error() == mailcore::ErrorNone) {
        _completionBlock(nil, MCO_TO_OBJC(op->result()));
    }
    else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

@end

@implementation MCOActiveSyncPingOperation {
    void (^_completionBlock)(NSError *error, MCOActiveSyncPingResult *result);
}

+ (void) load
{
    MCORegisterClass(self, &typeid(mailcore::ActiveSyncPingOperation));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [[[self alloc] initWithMCOperation:(mailcore::Operation *) object] autorelease];
}

- (void) dealloc
{
    [_completionBlock release];
    [super dealloc];
}

- (void) start:(void (^)(NSError *error, MCOActiveSyncPingResult *result))completionBlock
{
    _completionBlock = [completionBlock copy];
    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;

    mailcore::ActiveSyncPingOperation * op = (mailcore::ActiveSyncPingOperation *) [self mco_mcObject];
    if (op->error() == mailcore::ErrorNone) {
        _completionBlock(nil, MCO_TO_OBJC(op->result()));
    }
    else {
        _completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
    [_completionBlock release];
    _completionBlock = nil;
}

@end

#undef nativeType
