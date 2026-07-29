#include "MCActiveSyncMove.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncMove::init()
{
    mSourceMessageID = NULL;
    mSourceFolderID = NULL;
    mDestinationFolderID = NULL;
}

ActiveSyncMove::ActiveSyncMove()
{
    init();
}

ActiveSyncMove::~ActiveSyncMove()
{
    MC_SAFE_RELEASE(mSourceMessageID);
    MC_SAFE_RELEASE(mSourceFolderID);
    MC_SAFE_RELEASE(mDestinationFolderID);
}

void ActiveSyncMove::setSourceMessageID(String * value)
{
    MC_SET_STRING_FIELD(mSourceMessageID, value);
}

String * ActiveSyncMove::sourceMessageID()
{
    MC_GET_STRING_FIELD(mSourceMessageID);
}

void ActiveSyncMove::setSourceFolderID(String * value)
{
    MC_SET_STRING_FIELD(mSourceFolderID, value);
}

String * ActiveSyncMove::sourceFolderID()
{
    MC_GET_STRING_FIELD(mSourceFolderID);
}

void ActiveSyncMove::setDestinationFolderID(String * value)
{
    MC_SET_STRING_FIELD(mDestinationFolderID, value);
}

String * ActiveSyncMove::destinationFolderID()
{
    MC_GET_STRING_FIELD(mDestinationFolderID);
}

Object * ActiveSyncMove::copy()
{
    ActiveSyncMove * result = new ActiveSyncMove();
    result->setSourceMessageID(sourceMessageID());
    result->setSourceFolderID(sourceFolderID());
    result->setDestinationFolderID(destinationFolderID());
    return result;
}

String * ActiveSyncMove::description()
{
    return String::stringWithUTF8Format("<%s:%p %s>", className()->UTF8Characters(), this, MCUTF8(mSourceMessageID));
}
