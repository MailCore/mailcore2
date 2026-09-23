#import "MCOActiveSyncPrivate.h"

#define nativeType mailcore::ActiveSyncAttachmentData
MCO_DEFINE_ACTIVE_SYNC_WRAPPER(MCOActiveSyncAttachmentData, mailcore::ActiveSyncAttachmentData)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOActiveSyncItemOperationsStatus, mailcore::ActiveSyncItemOperationsStatus, setStatus, status)
MCO_OBJC_SYNTHESIZE_STRING(setFileReference, fileReference)
MCO_OBJC_SYNTHESIZE_STRING(setRange, range)
MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setTotal, total)
MCO_OBJC_SYNTHESIZE_DATA(setData, data)
MCO_END_ACTIVE_SYNC_WRAPPER
#undef nativeType
