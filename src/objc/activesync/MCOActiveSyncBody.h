#ifndef MAILCORE_MCOACTIVESYNCBODY_H

#define MAILCORE_MCOACTIVESYNCBODY_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncBody : NSObject <NSCopying>
@property (nonatomic, assign) MCOActiveSyncBodyType type;
@property (nonatomic, copy) NSData * data;
@property (nonatomic, assign) uint32_t estimatedDataSize;
@property (nonatomic, assign, getter=isTruncated) BOOL truncated;
@property (nonatomic, assign) MCOActiveSyncBodyType nativeBodyType;
@property (nonatomic, copy) NSString * mimeType;
@property (nonatomic, copy) NSString * preview;
@property (nonatomic, copy) NSArray * attachments;
@end

#endif
