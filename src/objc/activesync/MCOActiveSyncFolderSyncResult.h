#ifndef MAILCORE_MCOACTIVESYNCFOLDERSYNCRESULT_H

#define MAILCORE_MCOACTIVESYNCFOLDERSYNCRESULT_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncFolderSyncResult : NSObject <NSCopying>
@property (nonatomic, copy) NSString * syncKey;
@property (nonatomic, assign) MCOActiveSyncFolderSyncStatus status;
@property (nonatomic, copy) NSArray * added;
@property (nonatomic, copy) NSArray * updated;
@property (nonatomic, copy) NSArray * deleted;
@end

#endif
