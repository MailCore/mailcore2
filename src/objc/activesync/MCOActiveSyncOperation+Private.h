#ifndef MAILCORE_MCOACTIVESYNCOPERATION_PRIVATE_H

#define MAILCORE_MCOACTIVESYNCOPERATION_PRIVATE_H

#import "MCOActiveSyncOperation.h"

@class MCOActiveSyncSession;

@interface MCOActiveSyncOperation (Private)
- (void) setSession:(MCOActiveSyncSession *)session;
- (MCOActiveSyncSession *) session;
@end

#endif
