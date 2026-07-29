#import "MCOActiveSyncPrivate.h"

#define nativeType mailcore::ActiveSyncSyncRequest
MCO_DEFINE_ACTIVE_SYNC_WRAPPER(MCOActiveSyncSyncRequest, mailcore::ActiveSyncSyncRequest)
MCO_OBJC_SYNTHESIZE_STRING(setCollectionID, collectionID)
MCO_OBJC_SYNTHESIZE_STRING(setSyncKey, syncKey)
MCO_OBJC_SYNTHESIZE_STRING(setCollectionClass, collectionClass)
MCO_OBJC_SYNTHESIZE_BOOL(setGetChanges, getChanges)
MCO_OBJC_SYNTHESIZE_BOOL(setDeletesAsMoves, deletesAsMoves)
MCO_OBJC_SYNTHESIZE_SCALAR(MCOActiveSyncFilterType, mailcore::ActiveSyncFilterType, setFilterType, filterType)
MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setConflict, conflict)
MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setWindowSize, windowSize)

- (BOOL) hasFilterType
{
    return MCO_NATIVE_INSTANCE->hasFilterType();
}

- (BOOL) hasConflict
{
    return MCO_NATIVE_INSTANCE->hasConflict();
}

- (void) setBodyPreferenceWithType:(MCOActiveSyncBodyType)type truncationSize:(uint32_t)truncationSize
{
    MCO_NATIVE_INSTANCE->setBodyPreference((mailcore::ActiveSyncBodyType) type, truncationSize);
}

MCO_END_ACTIVE_SYNC_WRAPPER
#undef nativeType
