#ifndef MAILCORE_MCOACTIVESYNCATTACHMENTDATA_H

#define MAILCORE_MCOACTIVESYNCATTACHMENTDATA_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncAttachmentData : NSObject <NSCopying>
@property (nonatomic, assign) MCOActiveSyncItemOperationsStatus status;
@property (nonatomic, copy) NSString * fileReference;
@property (nonatomic, copy) NSString * range;
@property (nonatomic, assign) uint32_t total;
@property (nonatomic, copy) NSData * data;
@end

#endif
