#include "MCActiveSyncFolderMutationResult.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncFolderMutationResult::init()
{
    mSyncKey = NULL;
    mServerID = NULL;
    mStatus = ActiveSyncFolderMutationStatusUnknown;
}

ActiveSyncFolderMutationResult::ActiveSyncFolderMutationResult()
{
    init();
}

ActiveSyncFolderMutationResult::~ActiveSyncFolderMutationResult()
{
    MC_SAFE_RELEASE(mSyncKey);
    MC_SAFE_RELEASE(mServerID);
}

void ActiveSyncFolderMutationResult::setSyncKey(String * value)
{
    MC_SET_STRING_FIELD(mSyncKey, value);
}

String * ActiveSyncFolderMutationResult::syncKey()
{
    MC_GET_STRING_FIELD(mSyncKey);
}

void ActiveSyncFolderMutationResult::setServerID(String * value)
{
    MC_SET_STRING_FIELD(mServerID, value);
}

String * ActiveSyncFolderMutationResult::serverID()
{
    MC_GET_STRING_FIELD(mServerID);
}

void ActiveSyncFolderMutationResult::setFolderID(String * value)
{
    setServerID(value);
}

String * ActiveSyncFolderMutationResult::folderID()
{
    return serverID();
}

void ActiveSyncFolderMutationResult::setStatus(ActiveSyncFolderMutationStatus value)
{
    mStatus = value;
}

ActiveSyncFolderMutationStatus ActiveSyncFolderMutationResult::status()
{
    return mStatus;
}

Object * ActiveSyncFolderMutationResult::copy()
{
    ActiveSyncFolderMutationResult * result = new ActiveSyncFolderMutationResult();
    result->setSyncKey(syncKey());
    result->setServerID(serverID());
    result->setStatus(status());
    return result;
}

String * ActiveSyncFolderMutationResult::description()
{
    return String::stringWithUTF8Format("<%s:%p %s>", className()->UTF8Characters(), this, MCUTF8(mServerID));
}
