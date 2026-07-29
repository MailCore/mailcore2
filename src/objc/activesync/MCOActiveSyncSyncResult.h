#ifndef MAILCORE_MCOACTIVESYNCSYNCRESULT_H

#define MAILCORE_MCOACTIVESYNCSYNCRESULT_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncSyncResult : NSObject <NSCopying>
@property (nonatomic, copy) NSString * syncKey;
@property (nonatomic, assign) MCOActiveSyncSyncStatus status;
@property (nonatomic, assign) BOOL moreAvailable;
@property (nonatomic, assign) BOOL emptyResponse;
@property (nonatomic, assign) BOOL syncKeyFromResponse;
@property (nonatomic, copy) NSArray * added;
@property (nonatomic, copy) NSArray * changed;
@property (nonatomic, copy) NSArray * deleted;
@end

#endif
