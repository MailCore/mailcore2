#ifndef MAILCORE_MCOACTIVESYNCPINGRESULT_H

#define MAILCORE_MCOACTIVESYNCPINGRESULT_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncPingResult : NSObject <NSCopying>
@property (nonatomic, assign) MCOActiveSyncPingStatus status;
@property (nonatomic, copy) NSArray * changedCollectionIDs;
@end

#endif
