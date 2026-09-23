#ifndef MAILCORE_MCOACTIVESYNCMOVE_H

#define MAILCORE_MCOACTIVESYNCMOVE_H

#import <Foundation/Foundation.h>

@interface MCOActiveSyncMove : NSObject <NSCopying>
@property (nonatomic, copy) NSString * sourceMessageID;
@property (nonatomic, copy) NSString * sourceFolderID;
@property (nonatomic, copy) NSString * destinationFolderID;
@end

#endif
