#ifndef MAILCORE_MCOACTIVESYNCITEMESTIMATERESULT_H

#define MAILCORE_MCOACTIVESYNCITEMESTIMATERESULT_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncItemEstimateResult : NSObject <NSCopying>
@property (nonatomic, assign) MCOActiveSyncItemEstimateStatus status;
@property (nonatomic, assign) MCOActiveSyncItemEstimateStatus collectionStatus;
@property (nonatomic, assign) uint32_t estimate;
@property (nonatomic, assign) BOOL emptyResponse;
@end

#endif
