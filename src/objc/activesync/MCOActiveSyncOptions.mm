#import "MCOActiveSyncPrivate.h"

#define nativeType mailcore::ActiveSyncOptions
MCO_DEFINE_ACTIVE_SYNC_WRAPPER(MCOActiveSyncOptions, mailcore::ActiveSyncOptions)
MCO_OBJC_SYNTHESIZE_ARRAY(setProtocolVersions, protocolVersions)
MCO_OBJC_SYNTHESIZE_ARRAY(setCommands, commands)
MCO_END_ACTIVE_SYNC_WRAPPER
#undef nativeType
