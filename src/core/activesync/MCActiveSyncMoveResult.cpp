#include "MCActiveSyncMoveResult.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncMoveResult::init()
{
    mStatus = ActiveSyncMoveStatusUnknown;
    mResponses = NULL;
}

ActiveSyncMoveResult::ActiveSyncMoveResult()
{
    init();
}

ActiveSyncMoveResult::~ActiveSyncMoveResult()
{
    MC_SAFE_RELEASE(mResponses);
}

void ActiveSyncMoveResult::setStatus(ActiveSyncMoveStatus value)
{
    mStatus = value;
}

ActiveSyncMoveStatus ActiveSyncMoveResult::status()
{
    return mStatus;
}

void ActiveSyncMoveResult::setResponses(Array * value)
{
    MC_SAFE_REPLACE_RETAIN(Array, mResponses, value);
}

Array * ActiveSyncMoveResult::responses()
{
    return mResponses;
}

Object * ActiveSyncMoveResult::copy()
{
    ActiveSyncMoveResult * result = new ActiveSyncMoveResult();
    result->setStatus(status());
    result->setResponses(responses());
    return result;
}

String * ActiveSyncMoveResult::description()
{
    return String::stringWithUTF8Format("<%s:%p>", className()->UTF8Characters(), this);
}
