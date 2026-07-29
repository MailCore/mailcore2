#import "MCOActiveSyncPrivate.h"

#define nativeType mailcore::ActiveSyncPingResult
MCO_DEFINE_ACTIVE_SYNC_WRAPPER(MCOActiveSyncPingResult, mailcore::ActiveSyncPingResult)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOActiveSyncPingStatus, mailcore::ActiveSyncPingStatus, setStatus, status)
MCO_OBJC_SYNTHESIZE_ARRAY(setChangedCollectionIDs, changedCollectionIDs)
MCO_END_ACTIVE_SYNC_WRAPPER
#undef nativeType
