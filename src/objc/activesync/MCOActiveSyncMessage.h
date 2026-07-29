#ifndef MAILCORE_MCOACTIVESYNCMESSAGE_H

#define MAILCORE_MCOACTIVESYNCMESSAGE_H

#import <MailCore/MCOAbstractMessage.h>
#import <MailCore/MCOActiveSyncBody.h>

@interface MCOActiveSyncMessage : MCOAbstractMessage
@property (nonatomic, copy) NSString * serverID;
@property (nonatomic, copy) NSString * messageClass;
@property (nonatomic, assign) uint32_t estimatedSize;
@property (nonatomic, assign, getter=isRead) BOOL read;
@property (nonatomic, assign, getter=isFlagged) BOOL flagged;
@property (nonatomic, copy) NSData * messageData;
@property (nonatomic, retain) MCOActiveSyncBody * body;
@end

#endif
