#include "MCActiveSyncSyncResult.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncSyncResult::init()
{
    mSyncKey = NULL;
    mStatus = ActiveSyncSyncStatusUnknown;
    mMoreAvailable = false;
    mEmptyResponse = false;
    mSyncKeyFromResponse = false;
    mAdded = NULL;
    mChanged = NULL;
    mDeleted = NULL;
}

ActiveSyncSyncResult::ActiveSyncSyncResult()
{
    init();
}

ActiveSyncSyncResult::~ActiveSyncSyncResult()
{
    MC_SAFE_RELEASE(mSyncKey);
    MC_SAFE_RELEASE(mAdded);
    MC_SAFE_RELEASE(mChanged);
    MC_SAFE_RELEASE(mDeleted);
}

void ActiveSyncSyncResult::setSyncKey(String * value)
{
    MC_SET_STRING_FIELD(mSyncKey, value);
}

String * ActiveSyncSyncResult::syncKey()
{
    MC_GET_STRING_FIELD(mSyncKey);
}

void ActiveSyncSyncResult::setStatus(ActiveSyncSyncStatus value)
{
    mStatus = value;
}

ActiveSyncSyncStatus ActiveSyncSyncResult::status()
{
    return mStatus;
}

void ActiveSyncSyncResult::setMoreAvailable(bool value)
{
    mMoreAvailable = value;
}

bool ActiveSyncSyncResult::moreAvailable()
{
    return mMoreAvailable;
}

void ActiveSyncSyncResult::setEmptyResponse(bool value)
{
    mEmptyResponse = value;
}

bool ActiveSyncSyncResult::emptyResponse()
{
    return mEmptyResponse;
}

void ActiveSyncSyncResult::setSyncKeyFromResponse(bool value)
{
    mSyncKeyFromResponse = value;
}

bool ActiveSyncSyncResult::syncKeyFromResponse()
{
    return mSyncKeyFromResponse;
}

void ActiveSyncSyncResult::setAdded(Array * /* ActiveSyncMessage */ value)
{
    MC_SET_OBJECT_FIELD(Array, mAdded, value);
}

Array * /* ActiveSyncMessage */ ActiveSyncSyncResult::added()
{
    MC_GET_OBJECT_FIELD(mAdded);
}

void ActiveSyncSyncResult::setChanged(Array * /* ActiveSyncMessage */ value)
{
    MC_SET_OBJECT_FIELD(Array, mChanged, value);
}

Array * /* ActiveSyncMessage */ ActiveSyncSyncResult::changed()
{
    MC_GET_OBJECT_FIELD(mChanged);
}

void ActiveSyncSyncResult::setDeleted(Array * /* String */ value)
{
    MC_SET_OBJECT_FIELD(Array, mDeleted, value);
}

Array * /* String */ ActiveSyncSyncResult::deleted()
{
    MC_GET_OBJECT_FIELD(mDeleted);
}

Object * ActiveSyncSyncResult::copy()
{
    ActiveSyncSyncResult * result = new ActiveSyncSyncResult();
    result->setSyncKey(syncKey());
    result->setStatus(status());
    result->setMoreAvailable(moreAvailable());
    result->setEmptyResponse(emptyResponse());
    result->setSyncKeyFromResponse(syncKeyFromResponse());
    result->setAdded(added());
    result->setChanged(changed());
    result->setDeleted(deleted());
    return result;
}

String * ActiveSyncSyncResult::description()
{
    return String::stringWithUTF8Format("<%s:%p status:%i>", className()->UTF8Characters(), this, status());
}
