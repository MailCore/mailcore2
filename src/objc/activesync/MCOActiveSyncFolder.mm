#import "MCOActiveSyncPrivate.h"

#define nativeType mailcore::ActiveSyncFolder
MCO_DEFINE_ACTIVE_SYNC_WRAPPER(MCOActiveSyncFolder, mailcore::ActiveSyncFolder)
MCO_OBJC_SYNTHESIZE_STRING(setServerID, serverID)
MCO_OBJC_SYNTHESIZE_STRING(setParentID, parentID)
MCO_OBJC_SYNTHESIZE_STRING(setDisplayName, displayName)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOActiveSyncFolderType, mailcore::ActiveSyncFolderType, setType, type)
MCO_END_ACTIVE_SYNC_WRAPPER
#undef nativeType
