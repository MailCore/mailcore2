#include "MCActiveSyncFolder.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncFolder::init()
{
    mServerID = NULL;
    mParentID = NULL;
    mDisplayName = NULL;
    mType = ActiveSyncFolderTypeUnknown;
}

ActiveSyncFolder::ActiveSyncFolder()
{
    init();
}

ActiveSyncFolder::~ActiveSyncFolder()
{
    MC_SAFE_RELEASE(mServerID);
    MC_SAFE_RELEASE(mParentID);
    MC_SAFE_RELEASE(mDisplayName);
}

void ActiveSyncFolder::setServerID(String * value)
{
    MC_SET_STRING_FIELD(mServerID, value);
}

String * ActiveSyncFolder::serverID()
{
    MC_GET_STRING_FIELD(mServerID);
}

void ActiveSyncFolder::setParentID(String * value)
{
    MC_SET_STRING_FIELD(mParentID, value);
}

String * ActiveSyncFolder::parentID()
{
    MC_GET_STRING_FIELD(mParentID);
}

void ActiveSyncFolder::setDisplayName(String * value)
{
    MC_SET_STRING_FIELD(mDisplayName, value);
}

String * ActiveSyncFolder::displayName()
{
    MC_GET_STRING_FIELD(mDisplayName);
}

void ActiveSyncFolder::setType(ActiveSyncFolderType value)
{
    mType = value;
}

ActiveSyncFolderType ActiveSyncFolder::type()
{
    return mType;
}

Object * ActiveSyncFolder::copy()
{
    ActiveSyncFolder * result = new ActiveSyncFolder();
    result->setServerID(serverID());
    result->setParentID(parentID());
    result->setDisplayName(displayName());
    result->setType(type());
    return result;
}

String * ActiveSyncFolder::description()
{
    return String::stringWithUTF8Format("<%s:%p %s>", className()->UTF8Characters(), this, MCUTF8(mDisplayName));
}
