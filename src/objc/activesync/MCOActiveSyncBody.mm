#import "MCOActiveSyncPrivate.h"

#define nativeType mailcore::ActiveSyncBody
MCO_DEFINE_ACTIVE_SYNC_WRAPPER(MCOActiveSyncBody, mailcore::ActiveSyncBody)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOActiveSyncBodyType, mailcore::ActiveSyncBodyType, setType, type)
MCO_OBJC_SYNTHESIZE_DATA(setData, data)
MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setEstimatedDataSize, estimatedDataSize)
MCO_OBJC_SYNTHESIZE_BOOL(setTruncated, isTruncated)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOActiveSyncBodyType, mailcore::ActiveSyncBodyType, setNativeBodyType, nativeBodyType)
MCO_OBJC_SYNTHESIZE_STRING(setMimeType, mimeType)
MCO_OBJC_SYNTHESIZE_STRING(setPreview, preview)
MCO_OBJC_SYNTHESIZE_ARRAY(setAttachments, attachments)
MCO_END_ACTIVE_SYNC_WRAPPER
#undef nativeType
