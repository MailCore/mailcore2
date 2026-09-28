#ifndef MAILCORE_MCOACTIVESYNCFOLDERMUTATIONRESULT_H

#define MAILCORE_MCOACTIVESYNCFOLDERMUTATIONRESULT_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOActiveSyncTypes.h>

@interface MCOActiveSyncFolderMutationResult : NSObject <NSCopying>
@property (nonatomic, copy) NSString * syncKey;
@property (nonatomic, copy) NSString * serverID;
@property (nonatomic, copy) NSString * folderID;
@property (nonatomic, assign) MCOActiveSyncFolderMutationStatus status;
@end

#endif
