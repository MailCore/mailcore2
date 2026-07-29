#ifndef MAILCORE_MCOACTIVESYNCATTACHMENT_H

#define MAILCORE_MCOACTIVESYNCATTACHMENT_H

#import <MailCore/MCOAbstractPart.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncAttachment : MCOAbstractPart
@property (nonatomic, assign) MCOActiveSyncAttachmentMethod method;
@property (nonatomic, assign) uint32_t estimatedDataSize;
@end

#endif
