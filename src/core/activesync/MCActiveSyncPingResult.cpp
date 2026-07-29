#include "MCActiveSyncPingResult.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

ActiveSyncPingResult::ActiveSyncPingResult()
{
    mStatus = ActiveSyncPingStatusUnknown;
    mChangedCollectionIDs = NULL;
}

ActiveSyncPingResult::~ActiveSyncPingResult()
{
    MC_SAFE_RELEASE(mChangedCollectionIDs);
}

void ActiveSyncPingResult::setStatus(ActiveSyncPingStatus value)
{
    mStatus = value;
}

ActiveSyncPingStatus ActiveSyncPingResult::status()
{
    return mStatus;
}

void ActiveSyncPingResult::setChangedCollectionIDs(Array * /* String */ value)
{
    MC_SET_OBJECT_FIELD(Array, mChangedCollectionIDs, value);
}

Array * /* String */ ActiveSyncPingResult::changedCollectionIDs()
{
    return mChangedCollectionIDs;
}

Object * ActiveSyncPingResult::copy()
{
    ActiveSyncPingResult * result = new ActiveSyncPingResult();
    result->setStatus(status());
    result->setChangedCollectionIDs(changedCollectionIDs());
    return result;
}
