//
//  MCOIMAPSession.m
//  mailcore2
//
//  Created by Matt Ronge on 1/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPSession.h"

#import "MCOOperation.h"
#import "MCOOperation+Private.h"
#import "MCOObjectWrapper.h"
#import "MCOIMAPOperation.h"
#import "MCOIMAPFetchFoldersOperation.h"

#import "MCOUtils.h"

#import <MailCore/MCAsync.h>


using namespace mailcore;

@implementation MCOIMAPSession {
    IMAPAsyncSession * _session;
}

#define nativeType mailcore::IMAPAsyncSession

- (mailcore::Object *) mco_mcObject
{
    return _session;
}

- (id)init {
    self = [super init];
    if (self) {
        _session = new IMAPAsyncSession();
    }
    return self;
}

- (void)dealloc {
    _session->release();
    [super dealloc];
}

MCO_OBJC_SYNTHESIZE_STRING(setHostname, hostname)
MCO_OBJC_SYNTHESIZE_SCALAR(unsigned int, unsigned int, setPort, port)
MCO_OBJC_SYNTHESIZE_STRING(setUsername, username)
MCO_OBJC_SYNTHESIZE_STRING(setPassword, password)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOAuthType, mailcore::AuthType, setAuthType, authType)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOConnectionType, mailcore::ConnectionType, setConnectionType, connectionType)
MCO_OBJC_SYNTHESIZE_SCALAR(NSTimeInterval, time_t, setTimeout, timeout)
MCO_OBJC_SYNTHESIZE_BOOL(setCheckCertificateEnabled, isCheckCertificateEnabled)
MCO_OBJC_SYNTHESIZE_BOOL(setVoIPEnabled, isVoIPEnabled)
MCO_OBJC_SYNTHESIZE_SCALAR(char, char, setDelimiter, delimiter)
MCO_OBJC_SYNTHESIZE_SCALAR(BOOL, BOOL, setAllowsFolderConcurrentAccessEnabled, allowsFolderConcurrentAccessEnabled)
MCO_OBJC_SYNTHESIZE_SCALAR(unsigned int, unsigned int, setMaximumConnections, maximumConnections)

#pragma mark - Operations

- (MCOIMAPFolderInfoOperation *) folderInfoOperation:(NSString *)folder
{
    IMAPFolderInfoOperation * coreOp = MCO_NATIVE_INSTANCE->folderInfoOperation([folder mco_mcString]);
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPFetchFoldersOperation *) fetchSubscribedFoldersOperation
{
    IMAPOperation *coreOp = MCO_NATIVE_INSTANCE->fetchSubscribedFoldersOperation();
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPFetchFoldersOperation *)fetchAllFoldersOperation {
    IMAPOperation *coreOp = MCO_NATIVE_INSTANCE->fetchAllFoldersOperation();
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPOperation *) renameFolderOperation:(NSString *)folder otherName:(NSString *)otherName
{
    IMAPOperation *coreOp = MCO_NATIVE_INSTANCE->renameFolderOperation([folder mco_mcString], [otherName mco_mcString]);
    return [[[MCOIMAPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

- (MCOIMAPOperation *) deleteFolderOperation:(NSString *)folder
{
    IMAPOperation *coreOp = MCO_NATIVE_INSTANCE->deleteFolderOperation([folder mco_mcString]);
    return [[[MCOIMAPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

- (MCOIMAPOperation *) createFolderOperation:(NSString *)folder
{
    IMAPOperation *coreOp = MCO_NATIVE_INSTANCE->createFolderOperation([folder mco_mcString]);
    return [[[MCOIMAPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

- (MCOIMAPOperation *) subscribeFolderOperation:(NSString *)folder
{
    IMAPOperation *coreOp = MCO_NATIVE_INSTANCE->subscribeFolderOperation([folder mco_mcString]);
    return [[[MCOIMAPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

- (MCOIMAPOperation *) unsubscribeFolderOperation:(NSString *)folder
{
    IMAPOperation *coreOp = MCO_NATIVE_INSTANCE->unsubscribeFolderOperation([folder mco_mcString]);
    return [[[MCOIMAPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

- (MCOIMAPAppendMessageOperation *)appendMessageOperationWithFolder:(NSString *)folder
                                                        messageData:(NSData *)messageData
                                                              flags:(MCOMessageFlag)flags
{
    IMAPAppendMessageOperation * coreOp = MCO_NATIVE_INSTANCE->appendMessageOperation([folder mco_mcString],
                                                                                      [messageData mco_mcData],
                                                                                      (MessageFlag) flags);
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPCopyMessagesOperation *)copyMessagesOperationWithFolder:(NSString *)folder
                                                             uids:(MCOIndexSet *)uids
                                                       destFolder:(NSString *)destFolder
{
    IMAPCopyMessagesOperation * coreOp = MCO_NATIVE_INSTANCE->copyMessagesOperation([folder mco_mcString],
                                                                                    MCO_FROM_OBJC(IndexSet, uids),
                                                                                    [destFolder mco_mcString]);
    return MCO_TO_OBJC(coreOp);
}


- (MCOIMAPOperation *) expungeOperation:(NSString *)folder
{
    IMAPOperation *coreOp = MCO_NATIVE_INSTANCE->expungeOperation([folder mco_mcString]);
    return [[[MCOIMAPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

- (MCOIMAPFetchMessagesOperation *) fetchMessagesByUIDOperationWithFolder:(NSString *)folder
                                                              requestKind:(MCOIMAPMessagesRequestKind)requestKind
                                                                     uids:(MCOIndexSet *)uids
{
    NSLog(@"1:%p", MCO_NATIVE_INSTANCE);
    NSLog(@"2:%p", [folder mco_mcString]);
    NSLog(@"3:%p", MCO_FROM_OBJC(IndexSet, uids));
    IMAPFetchMessagesOperation * coreOp = MCO_NATIVE_INSTANCE->fetchMessagesByUIDOperation([folder mco_mcString],
                                                                                           (IMAPMessagesRequestKind) requestKind,
                                                                                           MCO_FROM_OBJC(IndexSet, uids));
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPFetchMessagesOperation *) fetchMessagesByNumberOperationWithFolder:(NSString *)folder
                                                                 requestKind:(MCOIMAPMessagesRequestKind)requestKind
                                                                     numbers:(MCOIndexSet *)numbers
{
    IMAPFetchMessagesOperation * coreOp = MCO_NATIVE_INSTANCE->fetchMessagesByNumberOperation([folder mco_mcString],
                                                                                              (IMAPMessagesRequestKind) requestKind,
                                                                                              MCO_FROM_OBJC(IndexSet, numbers));
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPFetchMessagesOperation *) syncMessagesByUIDWithFolder:(NSString *)folder
                                                    requestKind:(MCOIMAPMessagesRequestKind)requestKind
                                                           uids:(MCOIndexSet *)uids
                                                         modSeq:(uint64_t)modSeq
{
    IMAPFetchMessagesOperation * coreOp = MCO_NATIVE_INSTANCE->syncMessagesByUID([folder mco_mcString],
                                                                                 (IMAPMessagesRequestKind) requestKind,
                                                                                 MCO_FROM_OBJC(IndexSet, uids),
                                                                                 modSeq);
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPFetchContentOperation *) fetchMessageByUIDOperationWithFolder:(NSString *)folder
                                                                    uid:(uint32_t)uid
                                                                 urgent:(BOOL)urgent
{
    IMAPFetchContentOperation * coreOp = MCO_NATIVE_INSTANCE->fetchMessageByUIDOperation([folder mco_mcString], uid, urgent);
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPFetchContentOperation *) fetchMessageByUIDOperationWithFolder:(NSString *)folder
                                                                    uid:(uint32_t)uid
{
    return [self fetchMessageByUIDOperationWithFolder:folder uid:uid urgent:NO];
}

- (MCOIMAPFetchContentOperation *) fetchMessageAttachmentByUIDOperationWithFolder:(NSString *)folder
                                                                              uid:(uint32_t)uid
                                                                           partID:(NSString *)partID
                                                                         encoding:(MCOEncoding)encoding
                                                                           urgent:(BOOL)urgent
{
    IMAPFetchContentOperation * coreOp = MCO_NATIVE_INSTANCE->fetchMessageAttachmentByUIDOperation([folder mco_mcString],
                                                                                                   uid,
                                                                                                   [partID mco_mcString],
                                                                                                   (Encoding) encoding,
                                                                                                   urgent);
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPFetchContentOperation *) fetchMessageAttachmentByUIDOperationWithFolder:(NSString *)folder
                                                                              uid:(uint32_t)uid
                                                                           partID:(NSString *)partID
                                                                         encoding:(MCOEncoding)encoding
{
    return [self fetchMessageAttachmentByUIDOperationWithFolder:folder uid:uid partID:partID encoding:encoding urgent:NO];
}

- (MCOIMAPOperation *) storeFlagsOperationWithFolder:(NSString *)folder
                                                uids:(MCOIndexSet *)uids
                                                kind:(MCOIMAPStoreFlagsRequestKind)kind
                                               flags:(MCOMessageFlag)flags
{
    IMAPOperation * coreOp = MCO_NATIVE_INSTANCE->storeFlagsOperation([folder mco_mcString],
                                                                      MCO_FROM_OBJC(IndexSet, uids),
                                                                      (IMAPStoreFlagsRequestKind) kind,
                                                                      (MessageFlag) flags);
    return [[[MCOIMAPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

- (MCOIMAPOperation *) storeLabelsOperationWithFolder:(NSString *)folder
                                                 uids:(MCOIndexSet *)uids
                                                 kind:(MCOIMAPStoreFlagsRequestKind)kind
                                               labels:(NSArray *)labels
{
    IMAPOperation * coreOp = MCO_NATIVE_INSTANCE->storeLabelsOperation([folder mco_mcString],
                                                                       MCO_FROM_OBJC(IndexSet, uids),
                                                                       (IMAPStoreFlagsRequestKind) kind,
                                                                       MCO_FROM_OBJC(Array, labels));
    return [[[MCOIMAPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

- (MCOIMAPSearchOperation *) searchOperationWithFolder:(NSString *)folder
                                                  kind:(MCOIMAPSearchKind)kind
                                          searchString:(NSString *)searchString
{
    IMAPSearchOperation * coreOp = MCO_NATIVE_INSTANCE->searchOperation([folder mco_mcString],
                                                                        (IMAPSearchKind) kind,
                                                                        [searchString mco_mcString]);
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPSearchOperation *) searchExpressionOperationWithFolder:(NSString *)folder
                                                      expression:(MCOIMAPSearchExpression *)expression
{
    IMAPSearchOperation * coreOp = MCO_NATIVE_INSTANCE->searchOperation([folder mco_mcString],
                                                                        MCO_FROM_OBJC(IMAPSearchExpression, expression));
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPIdleOperation *) idleOperationWithFolder:(NSString *)folder
                                      lastKnownUID:(uint32_t)lastKnownUID
{
    IMAPIdleOperation * coreOp = MCO_NATIVE_INSTANCE->idleOperation([folder mco_mcString], lastKnownUID);
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPFetchNamespaceOperation *) fetchNamespaceOperation
{
    IMAPFetchNamespaceOperation * coreOp = MCO_NATIVE_INSTANCE->fetchNamespaceOperation();
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPIdentityOperation *) identityOperationWithVendor:(NSString *)vendor
                                                      name:(NSString *)name
                                                   version:(NSString *)version
{
    IMAPIdentityOperation * coreOp = MCO_NATIVE_INSTANCE->identityOperation([vendor mco_mcString],
                                                                            [name mco_mcString],
                                                                            [version mco_mcString]);
    return MCO_TO_OBJC(coreOp);
}

- (MCOIMAPOperation *)checkAccountOperation
{
    IMAPOperation *coreOp = MCO_NATIVE_INSTANCE->checkAccountOperation();
    return [[[MCOIMAPOperation alloc] initWithMCOperation:coreOp] autorelease];
}

- (MCOIMAPCapabilityOperation *) capabilityOperation
{
    IMAPCapabilityOperation * coreOp = MCO_NATIVE_INSTANCE->capabilityOperation();
    return MCO_TO_OBJC(coreOp);
}

@end
