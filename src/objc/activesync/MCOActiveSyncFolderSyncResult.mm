#import "MCOActiveSyncPrivate.h"

#define nativeType mailcore::ActiveSyncFolderSyncResult
MCO_DEFINE_ACTIVE_SYNC_WRAPPER(MCOActiveSyncFolderSyncResult, mailcore::ActiveSyncFolderSyncResult)
MCO_OBJC_SYNTHESIZE_STRING(setSyncKey, syncKey)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOActiveSyncFolderSyncStatus, mailcore::ActiveSyncFolderSyncStatus, setStatus, status)
MCO_OBJC_SYNTHESIZE_ARRAY(setAdded, added)
MCO_OBJC_SYNTHESIZE_ARRAY(setUpdated, updated)
MCO_OBJC_SYNTHESIZE_ARRAY(setDeleted, deleted)
MCO_END_ACTIVE_SYNC_WRAPPER
#undef nativeType
