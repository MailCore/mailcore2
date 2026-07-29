#ifndef MAILCORE_MCOACTIVESYNCFOLDER_H

#define MAILCORE_MCOACTIVESYNCFOLDER_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncFolder : NSObject <NSCopying>
@property (nonatomic, copy) NSString * serverID;
@property (nonatomic, copy) NSString * parentID;
@property (nonatomic, copy) NSString * displayName;
@property (nonatomic, assign) MCOActiveSyncFolderType type;
@end

#endif
