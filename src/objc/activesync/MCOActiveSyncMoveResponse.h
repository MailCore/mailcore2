#ifndef MAILCORE_MCOACTIVESYNCMOVERESPONSE_H

#define MAILCORE_MCOACTIVESYNCMOVERESPONSE_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncMoveResponse : NSObject <NSCopying>
@property (nonatomic, copy) NSString * sourceMessageID;
@property (nonatomic, copy) NSString * sourceFolderID;
@property (nonatomic, copy) NSString * destinationFolderID;
@property (nonatomic, copy) NSString * destinationMessageID;
@property (nonatomic, assign) MCOActiveSyncMoveStatus status;
@end

#endif
