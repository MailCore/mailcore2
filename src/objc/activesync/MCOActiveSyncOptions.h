#ifndef MAILCORE_MCOACTIVESYNCOPTIONS_H

#define MAILCORE_MCOACTIVESYNCOPTIONS_H

#import <Foundation/Foundation.h>

@interface MCOActiveSyncOptions : NSObject <NSCopying>
@property (nonatomic, copy) NSArray * protocolVersions;
@property (nonatomic, copy) NSArray * commands;
@end

#endif
