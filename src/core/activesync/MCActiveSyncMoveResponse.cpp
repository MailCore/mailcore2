#include "MCActiveSyncMoveResponse.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncMoveResponse::init()
{
    mSourceMessageID = NULL;
    mSourceFolderID = NULL;
    mDestinationFolderID = NULL;
    mDestinationMessageID = NULL;
    mStatus = ActiveSyncMoveStatusUnknown;
}

ActiveSyncMoveResponse::ActiveSyncMoveResponse()
{
    init();
}

ActiveSyncMoveResponse::~ActiveSyncMoveResponse()
{
    MC_SAFE_RELEASE(mSourceMessageID);
    MC_SAFE_RELEASE(mSourceFolderID);
    MC_SAFE_RELEASE(mDestinationFolderID);
    MC_SAFE_RELEASE(mDestinationMessageID);
}

void ActiveSyncMoveResponse::setSourceMessageID(String * value)
{
    MC_SET_STRING_FIELD(mSourceMessageID, value);
}

String * ActiveSyncMoveResponse::sourceMessageID()
{
    MC_GET_STRING_FIELD(mSourceMessageID);
}

void ActiveSyncMoveResponse::setSourceFolderID(String * value)
{
    MC_SET_STRING_FIELD(mSourceFolderID, value);
}

String * ActiveSyncMoveResponse::sourceFolderID()
{
    MC_GET_STRING_FIELD(mSourceFolderID);
}

void ActiveSyncMoveResponse::setDestinationFolderID(String * value)
{
    MC_SET_STRING_FIELD(mDestinationFolderID, value);
}

String * ActiveSyncMoveResponse::destinationFolderID()
{
    MC_GET_STRING_FIELD(mDestinationFolderID);
}

void ActiveSyncMoveResponse::setDestinationMessageID(String * value)
{
    MC_SET_STRING_FIELD(mDestinationMessageID, value);
}

String * ActiveSyncMoveResponse::destinationMessageID()
{
    MC_GET_STRING_FIELD(mDestinationMessageID);
}

void ActiveSyncMoveResponse::setStatus(ActiveSyncMoveStatus value)
{
    mStatus = value;
}

ActiveSyncMoveStatus ActiveSyncMoveResponse::status()
{
    return mStatus;
}

Object * ActiveSyncMoveResponse::copy()
{
    ActiveSyncMoveResponse * result = new ActiveSyncMoveResponse();
    result->setSourceMessageID(sourceMessageID());
    result->setSourceFolderID(sourceFolderID());
    result->setDestinationFolderID(destinationFolderID());
    result->setDestinationMessageID(destinationMessageID());
    result->setStatus(status());
    return result;
}

String * ActiveSyncMoveResponse::description()
{
    return String::stringWithUTF8Format("<%s:%p %s>", className()->UTF8Characters(), this, MCUTF8(mSourceMessageID));
}
