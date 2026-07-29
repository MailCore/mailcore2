#import "MCOActiveSyncPrivate.h"

#define nativeType mailcore::ActiveSyncSyncResult
MCO_DEFINE_ACTIVE_SYNC_WRAPPER(MCOActiveSyncSyncResult, mailcore::ActiveSyncSyncResult)
MCO_OBJC_SYNTHESIZE_STRING(setSyncKey, syncKey)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOActiveSyncSyncStatus, mailcore::ActiveSyncSyncStatus, setStatus, status)
MCO_OBJC_SYNTHESIZE_BOOL(setMoreAvailable, moreAvailable)
MCO_OBJC_SYNTHESIZE_BOOL(setEmptyResponse, emptyResponse)
MCO_OBJC_SYNTHESIZE_BOOL(setSyncKeyFromResponse, syncKeyFromResponse)
MCO_OBJC_SYNTHESIZE_ARRAY(setAdded, added)
MCO_OBJC_SYNTHESIZE_ARRAY(setChanged, changed)
MCO_OBJC_SYNTHESIZE_ARRAY(setDeleted, deleted)
MCO_END_ACTIVE_SYNC_WRAPPER
#undef nativeType
