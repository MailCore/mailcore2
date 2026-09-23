#ifndef MAILCORE_MCOACTIVESYNCOPERATION_H

#define MAILCORE_MCOACTIVESYNCOPERATION_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOOperation.h>
#import <MailCore/MCOActiveSyncOptions.h>
#import <MailCore/MCOActiveSyncFolderSyncResult.h>
#import <MailCore/MCOActiveSyncFolderMutationResult.h>
#import <MailCore/MCOActiveSyncSyncResult.h>
#import <MailCore/MCOActiveSyncProvisionResult.h>
#import <MailCore/MCOActiveSyncItemEstimateResult.h>
#import <MailCore/MCOActiveSyncMessage.h>
#import <MailCore/MCOActiveSyncAttachmentData.h>
#import <MailCore/MCOActiveSyncMoveResult.h>
#import <MailCore/MCOActiveSyncPingResult.h>

@class MCOActiveSyncSession;

NS_ASSUME_NONNULL_BEGIN

@interface MCOActiveSyncOperation : MCOOperation

- (void) start:(void (^)(NSError * __nullable error))completionBlock;

@end

@interface MCOActiveSyncOptionsOperation : MCOActiveSyncOperation

- (void) start:(void (^)(NSError * __nullable error, MCOActiveSyncOptions * __nullable options))completionBlock;

@end

@interface MCOActiveSyncFolderSyncOperation : MCOActiveSyncOperation

- (void) start:(void (^)(NSError * __nullable error, MCOActiveSyncFolderSyncResult * __nullable result))completionBlock;

@end

@interface MCOActiveSyncFolderMutationOperation : MCOActiveSyncOperation

- (void) start:(void (^)(NSError * __nullable error, MCOActiveSyncFolderMutationResult * __nullable result))completionBlock;

@end

@interface MCOActiveSyncSyncOperation : MCOActiveSyncOperation

- (void) start:(void (^)(NSError * __nullable error, MCOActiveSyncSyncResult * __nullable result))completionBlock;

@end

@interface MCOActiveSyncProvisionOperation : MCOActiveSyncOperation

- (void) start:(void (^)(NSError * __nullable error, MCOActiveSyncProvisionResult * __nullable result))completionBlock;

@end

@interface MCOActiveSyncItemEstimateOperation : MCOActiveSyncOperation

- (void) start:(void (^)(NSError * __nullable error, MCOActiveSyncItemEstimateResult * __nullable result))completionBlock;

@end

@interface MCOActiveSyncFetchMessageOperation : MCOActiveSyncOperation

- (void) start:(void (^)(NSError * __nullable error, MCOActiveSyncMessage * __nullable message))completionBlock;

@end

@interface MCOActiveSyncFetchAttachmentOperation : MCOActiveSyncOperation

- (void) start:(void (^)(NSError * __nullable error, MCOActiveSyncAttachmentData * __nullable data))completionBlock;

@end

@interface MCOActiveSyncMoveOperation : MCOActiveSyncOperation

- (void) start:(void (^)(NSError * __nullable error, MCOActiveSyncMoveResult * __nullable result))completionBlock;

@end

@interface MCOActiveSyncPingOperation : MCOActiveSyncOperation

- (void) start:(void (^)(NSError * __nullable error, MCOActiveSyncPingResult * __nullable result))completionBlock;

@end

NS_ASSUME_NONNULL_END

#endif
