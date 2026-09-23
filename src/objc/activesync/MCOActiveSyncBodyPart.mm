#import "MCOActiveSyncPrivate.h"

#define nativeType mailcore::ActiveSyncBodyPart
MCO_DEFINE_ACTIVE_SYNC_WRAPPER(MCOActiveSyncBodyPart, mailcore::ActiveSyncBodyPart)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOActiveSyncItemOperationsStatus, mailcore::ActiveSyncItemOperationsStatus, setStatus, status)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOActiveSyncBodyType, mailcore::ActiveSyncBodyType, setType, type)
MCO_OBJC_SYNTHESIZE_DATA(setData, data)
MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setEstimatedDataSize, estimatedDataSize)
MCO_OBJC_SYNTHESIZE_BOOL(setTruncated, isTruncated)
MCO_OBJC_SYNTHESIZE_STRING(setPreview, preview)
MCO_END_ACTIVE_SYNC_WRAPPER
#undef nativeType
