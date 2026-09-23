#ifndef MAILCORE_MCOACTIVESYNCBODYPART_H

#define MAILCORE_MCOACTIVESYNCBODYPART_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncBodyPart : NSObject <NSCopying>
@property (nonatomic, assign) MCOActiveSyncItemOperationsStatus status;
@property (nonatomic, assign) MCOActiveSyncBodyType type;
@property (nonatomic, copy) NSData * data;
@property (nonatomic, assign) uint32_t estimatedDataSize;
@property (nonatomic, assign, getter=isTruncated) BOOL truncated;
@property (nonatomic, copy) NSString * preview;
@end

#endif
