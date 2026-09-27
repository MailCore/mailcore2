//
//  MCOJMAPSession.mm
//  mailcore2
//

#import "MCOJMAPSession.h"

#include "MCAsyncJMAP.h"
#include "MCOperationQueueCallback.h"

#import "MCOJMAPOperation.h"
#import "MCOJMAPOperations.h"
#import "MCOOperation+Private.h"
#import "MCOUtils.h"

using namespace mailcore;

@interface MCOJMAPSession ()
- (void) _queueRunningChanged;
@end

class MCOJMAPCallbackBridge : public Object, public OperationQueueCallback {
public:
    MCOJMAPCallbackBridge(MCOJMAPSession * session)
    {
        mSession = session;
    }

    virtual void queueStartRunning()
    {
        [mSession _queueRunningChanged];
    }

    virtual void queueStoppedRunning()
    {
        [mSession _queueRunningChanged];
    }

private:
    MCOJMAPSession * mSession;
};

@implementation MCOJMAPSession {
    mailcore::JMAPAsyncSession * _session;
    MCOJMAPCallbackBridge * _callbackBridge;
    MCOJMAPOperationQueueRunningChangeBlock _operationQueueRunningChangeBlock;
}

#define nativeType mailcore::JMAPAsyncSession

- (mailcore::Object *) mco_mcObject
{
    return _session;
}

- (instancetype) init
{
    self = [super init];
    _session = new mailcore::JMAPAsyncSession();
    _callbackBridge = new MCOJMAPCallbackBridge(self);
    return self;
}

- (void) dealloc
{
    MC_SAFE_RELEASE(_callbackBridge);
    [_operationQueueRunningChangeBlock release];
    _session->setOperationQueueCallback(NULL);
    _session->release();
    [super dealloc];
}

MCO_OBJC_SYNTHESIZE_STRING(setSessionURL, sessionURL)
MCO_OBJC_SYNTHESIZE_STRING(setDomainOrEmail, domainOrEmail)
MCO_OBJC_SYNTHESIZE_STRING(setAccountID, accountID)
MCO_OBJC_SYNTHESIZE_STRING(setUsername, username)
MCO_OBJC_SYNTHESIZE_STRING(setOAuth2Token, OAuth2Token)
MCO_OBJC_SYNTHESIZE_SCALAR(NSTimeInterval, time_t, setTimeout, timeout)
MCO_OBJC_SYNTHESIZE_BOOL(setCheckCertificateEnabled, isCheckCertificateEnabled)
MCO_OBJC_SYNTHESIZE_SCALAR(dispatch_queue_t, dispatch_queue_t, setDispatchQueue, dispatchQueue)

- (int) lastHTTPStatus
{
    return MCO_NATIVE_INSTANCE->lastHTTPStatus();
}

- (NSString *) lastErrorMessage
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->lastErrorMessage());
}

- (BOOL) isOperationQueueRunning
{
    return MCO_NATIVE_INSTANCE->isOperationQueueRunning();
}

- (void) setOperationQueueRunningChangeBlock:(MCOJMAPOperationQueueRunningChangeBlock)operationQueueRunningChangeBlock
{
    [_operationQueueRunningChangeBlock release];
    _operationQueueRunningChangeBlock = [operationQueueRunningChangeBlock copy];

    if (_operationQueueRunningChangeBlock != nil) {
        _session->setOperationQueueCallback(_callbackBridge);
    }
    else {
        _session->setOperationQueueCallback(NULL);
    }
}

- (MCOJMAPOperationQueueRunningChangeBlock) operationQueueRunningChangeBlock
{
    return _operationQueueRunningChangeBlock;
}

- (void) _queueRunningChanged
{
    if (_operationQueueRunningChangeBlock != nil) {
        _operationQueueRunningChangeBlock();
    }
}

- (void) cancelAllOperations
{
    MCO_NATIVE_INSTANCE->cancelAllOperations();
}

- (id) _objcOperationFromNativeOperation:(mailcore::JMAPOperation *)op
{
    return MCO_TO_OBJC(op);
}

- (MCOJMAPOperation *) connectOperation
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->connectOperation()];
}

- (MCOJMAPOperation *) discoverOperation
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->discoverOperation()];
}

- (MCOJMAPFetchMailboxesOperation *) fetchMailboxesOperation
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->fetchMailboxesOperation()];
}

- (MCOJMAPQueryMessagesOperation *) queryMessagesWithTextOperation:(NSString *)text position:(unsigned int)position limit:(unsigned int)limit
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->queryMessagesWithTextOperation([text mco_mcString], position, limit)];
}

- (MCOJMAPQueryMessagesOperation *) queryMessagesInMailboxOperation:(NSString *)mailboxID position:(unsigned int)position limit:(unsigned int)limit
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->queryMessagesInMailboxOperation([mailboxID mco_mcString], position, limit)];
}

- (MCOJMAPFetchMessagesOperation *) fetchMessagesOperationWithMessageIDs:(NSArray<NSString *> *)messageIDs properties:(NSArray<NSString *> *)properties
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->fetchMessagesOperation([messageIDs mco_mcArray], [properties mco_mcArray])];
}

- (MCOJMAPUploadOperation *) uploadOperationWithData:(NSData *)data contentType:(NSString *)contentType accountID:(NSString *)accountID
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->uploadOperation([data mco_mcData], [contentType mco_mcString], [accountID mco_mcString])];
}

- (MCOJMAPDownloadOperation *) downloadOperationWithBlobID:(NSString *)blobID name:(NSString *)name accept:(NSString *)accept accountID:(NSString *)accountID
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->downloadOperation([blobID mco_mcString], [name mco_mcString], [accept mco_mcString], [accountID mco_mcString])];
}

- (MCOJMAPCreateDraftOperation *) createDraftOperationWithData:(NSData *)data mailboxID:(NSString *)mailboxID keywords:(NSArray<NSString *> *)keywords
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->createDraftOperation([data mco_mcData], [mailboxID mco_mcString], [keywords mco_mcArray])];
}

- (MCOJMAPOperation *) updateDraftOperationWithMessageID:(NSString *)messageID mailboxIDs:(NSArray<NSString *> *)mailboxIDs keywords:(NSArray<NSString *> *)keywords
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->updateDraftOperation([messageID mco_mcString], [mailboxIDs mco_mcArray], [keywords mco_mcArray])];
}

- (MCOJMAPOperation *) deleteDraftOperationWithMessageID:(NSString *)messageID
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->deleteDraftOperation([messageID mco_mcString])];
}

- (MCOJMAPSendOperation *) sendMessageOperationWithMessageID:(NSString *)messageID identityID:(NSString *)identityID
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->sendMessageOperation([messageID mco_mcString], [identityID mco_mcString])];
}

- (MCOJMAPSendOperation *) sendDataOperationWithData:(NSData *)data identityID:(NSString *)identityID
{
    return [self _objcOperationFromNativeOperation:MCO_NATIVE_INSTANCE->sendDataOperation([data mco_mcData], [identityID mco_mcString])];
}

@end
