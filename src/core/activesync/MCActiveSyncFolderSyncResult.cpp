#include "MCActiveSyncFolderSyncResult.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncFolderSyncResult::init()
{
    mSyncKey = NULL;
    mStatus = ActiveSyncFolderSyncStatusUnknown;
    mAdded = NULL;
    mUpdated = NULL;
    mDeleted = NULL;
}

ActiveSyncFolderSyncResult::ActiveSyncFolderSyncResult()
{
    init();
}

ActiveSyncFolderSyncResult::~ActiveSyncFolderSyncResult()
{
    MC_SAFE_RELEASE(mSyncKey);
    MC_SAFE_RELEASE(mAdded);
    MC_SAFE_RELEASE(mUpdated);
    MC_SAFE_RELEASE(mDeleted);
}

void ActiveSyncFolderSyncResult::setSyncKey(String * value)
{
    MC_SET_STRING_FIELD(mSyncKey, value);
}

String * ActiveSyncFolderSyncResult::syncKey()
{
    MC_GET_STRING_FIELD(mSyncKey);
}

void ActiveSyncFolderSyncResult::setStatus(ActiveSyncFolderSyncStatus value)
{
    mStatus = value;
}

ActiveSyncFolderSyncStatus ActiveSyncFolderSyncResult::status()
{
    return mStatus;
}

void ActiveSyncFolderSyncResult::setAdded(Array * /* ActiveSyncFolder */ value)
{
    MC_SET_OBJECT_FIELD(Array, mAdded, value);
}

Array * /* ActiveSyncFolder */ ActiveSyncFolderSyncResult::added()
{
    MC_GET_OBJECT_FIELD(mAdded);
}

void ActiveSyncFolderSyncResult::setUpdated(Array * /* ActiveSyncFolder */ value)
{
    MC_SET_OBJECT_FIELD(Array, mUpdated, value);
}

Array * /* ActiveSyncFolder */ ActiveSyncFolderSyncResult::updated()
{
    MC_GET_OBJECT_FIELD(mUpdated);
}

void ActiveSyncFolderSyncResult::setDeleted(Array * /* String */ value)
{
    MC_SET_OBJECT_FIELD(Array, mDeleted, value);
}

Array * /* String */ ActiveSyncFolderSyncResult::deleted()
{
    MC_GET_OBJECT_FIELD(mDeleted);
}

Object * ActiveSyncFolderSyncResult::copy()
{
    ActiveSyncFolderSyncResult * result = new ActiveSyncFolderSyncResult();
    result->setSyncKey(syncKey());
    result->setStatus(status());
    result->setAdded(added());
    result->setUpdated(updated());
    result->setDeleted(deleted());
    return result;
}

String * ActiveSyncFolderSyncResult::description()
{
    return String::stringWithUTF8Format("<%s:%p status:%i>", className()->UTF8Characters(), this, status());
}
