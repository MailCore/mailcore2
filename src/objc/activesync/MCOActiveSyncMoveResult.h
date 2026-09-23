#ifndef MAILCORE_MCOACTIVESYNCMOVERESULT_H

#define MAILCORE_MCOACTIVESYNCMOVERESULT_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncMoveResult : NSObject <NSCopying>
@property (nonatomic, assign) MCOActiveSyncMoveStatus status;
@property (nonatomic, copy) NSArray * responses;
@end

#endif
