#import "MCOActiveSyncPrivate.h"

#define nativeType mailcore::ActiveSyncMoveResult
MCO_DEFINE_ACTIVE_SYNC_WRAPPER(MCOActiveSyncMoveResult, mailcore::ActiveSyncMoveResult)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOActiveSyncMoveStatus, mailcore::ActiveSyncMoveStatus, setStatus, status)
MCO_OBJC_SYNTHESIZE_ARRAY(setResponses, responses)
MCO_END_ACTIVE_SYNC_WRAPPER
#undef nativeType
