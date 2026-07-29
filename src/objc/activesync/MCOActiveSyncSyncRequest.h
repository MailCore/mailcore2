#ifndef MAILCORE_MCOACTIVESYNCSYNCREQUEST_H

#define MAILCORE_MCOACTIVESYNCSYNCREQUEST_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncSyncRequest : NSObject <NSCopying>
@property (nonatomic, copy) NSString * collectionID;
@property (nonatomic, copy) NSString * syncKey;
@property (nonatomic, copy) NSString * collectionClass;
@property (nonatomic, assign) BOOL getChanges;
@property (nonatomic, assign) BOOL deletesAsMoves;
@property (nonatomic, assign) MCOActiveSyncFilterType filterType;
@property (nonatomic, readonly) BOOL hasFilterType;
@property (nonatomic, assign) uint32_t conflict;
@property (nonatomic, readonly) BOOL hasConflict;
@property (nonatomic, assign) uint32_t windowSize;
- (void) setBodyPreferenceWithType:(MCOActiveSyncBodyType)type truncationSize:(uint32_t)truncationSize;
@end

#endif
