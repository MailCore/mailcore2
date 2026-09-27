//
//  MCOJMAPOperations.h
//  mailcore2
//

#ifndef MAILCORE_MCOJMAPOPERATIONS_H

#define MAILCORE_MCOJMAPOPERATIONS_H

#import <MailCore/MCOJMAPOperation.h>

@class MCOJMAPBlobUpload;
@class MCOJMAPMessage;
@class MCOJMAPSubmission;

@interface MCOJMAPFetchMailboxesOperation : MCOJMAPOperation
- (void) start:(void (^)(NSError * error, NSArray * mailboxes))completionBlock;
@end

@interface MCOJMAPQueryMessagesOperation : MCOJMAPOperation
- (void) start:(void (^)(NSError * error, NSArray<NSString *> * messageIDs))completionBlock;
@end

@interface MCOJMAPFetchMessagesOperation : MCOJMAPOperation
- (void) start:(void (^)(NSError * error, NSArray<MCOJMAPMessage *> * messages))completionBlock;
@end

@interface MCOJMAPUploadOperation : MCOJMAPOperation
- (void) start:(void (^)(NSError * error, MCOJMAPBlobUpload * upload))completionBlock;
@end

@interface MCOJMAPDownloadOperation : MCOJMAPOperation
- (void) start:(void (^)(NSError * error, NSData * data))completionBlock;
@end

@interface MCOJMAPCreateDraftOperation : MCOJMAPOperation
- (void) start:(void (^)(NSError * error, MCOJMAPMessage * message))completionBlock;
@end

@interface MCOJMAPSendOperation : MCOJMAPOperation
- (void) start:(void (^)(NSError * error, MCOJMAPSubmission * submission))completionBlock;
@end

#endif
