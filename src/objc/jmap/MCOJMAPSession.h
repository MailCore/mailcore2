//
//  MCOJMAPSession.h
//  mailcore2
//

#ifndef MAILCORE_MCOJMAPSESSION_H

#define MAILCORE_MCOJMAPSESSION_H

#import <Foundation/Foundation.h>

@class MCOJMAPOperation;
@class MCOJMAPFetchMailboxesOperation;
@class MCOJMAPQueryMessagesOperation;
@class MCOJMAPFetchMessagesOperation;
@class MCOJMAPUploadOperation;
@class MCOJMAPDownloadOperation;
@class MCOJMAPCreateDraftOperation;
@class MCOJMAPSendOperation;

typedef void (^MCOJMAPOperationQueueRunningChangeBlock)(void);

@interface MCOJMAPSession : NSObject

@property (nonatomic, copy) NSString * sessionURL;
@property (nonatomic, copy) NSString * domainOrEmail;
@property (nonatomic, copy) NSString * accountID;
@property (nonatomic, copy) NSString * username;
@property (nonatomic, copy) NSString * OAuth2Token;
@property (nonatomic, assign) NSTimeInterval timeout;
@property (nonatomic, assign, getter=isCheckCertificateEnabled) BOOL checkCertificateEnabled;
@property (nonatomic, assign, readonly) int lastHTTPStatus;
@property (nonatomic, copy, readonly) NSString * lastErrorMessage;

#if OS_OBJECT_USE_OBJC
@property (nonatomic, retain) dispatch_queue_t dispatchQueue;
#else
@property (nonatomic, assign) dispatch_queue_t dispatchQueue;
#endif

@property (nonatomic, assign, readonly, getter=isOperationQueueRunning) BOOL operationQueueRunning;
@property (nonatomic, copy) MCOJMAPOperationQueueRunningChangeBlock operationQueueRunningChangeBlock;

- (void) cancelAllOperations;

- (MCOJMAPOperation *) connectOperation;
- (MCOJMAPOperation *) discoverOperation;
- (MCOJMAPFetchMailboxesOperation *) fetchMailboxesOperation;
- (MCOJMAPQueryMessagesOperation *) queryMessagesWithTextOperation:(NSString *)text position:(unsigned int)position limit:(unsigned int)limit;
- (MCOJMAPQueryMessagesOperation *) queryMessagesInMailboxOperation:(NSString *)mailboxID position:(unsigned int)position limit:(unsigned int)limit;
- (MCOJMAPFetchMessagesOperation *) fetchMessagesOperationWithMessageIDs:(NSArray<NSString *> *)messageIDs properties:(NSArray<NSString *> *)properties;
- (MCOJMAPUploadOperation *) uploadOperationWithData:(NSData *)data contentType:(NSString *)contentType accountID:(NSString *)accountID;
- (MCOJMAPDownloadOperation *) downloadOperationWithBlobID:(NSString *)blobID name:(NSString *)name accept:(NSString *)accept accountID:(NSString *)accountID;
- (MCOJMAPCreateDraftOperation *) createDraftOperationWithData:(NSData *)data mailboxID:(NSString *)mailboxID keywords:(NSArray<NSString *> *)keywords;
- (MCOJMAPOperation *) updateDraftOperationWithMessageID:(NSString *)messageID mailboxIDs:(NSArray<NSString *> *)mailboxIDs keywords:(NSArray<NSString *> *)keywords;
- (MCOJMAPOperation *) deleteDraftOperationWithMessageID:(NSString *)messageID;
- (MCOJMAPSendOperation *) sendMessageOperationWithMessageID:(NSString *)messageID identityID:(NSString *)identityID;
- (MCOJMAPSendOperation *) sendDataOperationWithData:(NSData *)data identityID:(NSString *)identityID;

@end

#endif
