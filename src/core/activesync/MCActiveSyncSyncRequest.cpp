#include "MCActiveSyncSyncRequest.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncSyncRequest::init()
{
    mCollectionID = NULL;
    mSyncKey = NULL;
    mCollectionClass = NULL;
    mGetChanges = true;
    mDeletesAsMoves = false;
    mHasFilterType = false;
    mFilterType = ActiveSyncFilterTypeNone;
    mHasConflict = false;
    mConflict = 0;
    mWindowSize = 0;
    mHasBodyPreference = false;
    mBodyPreferenceType = ActiveSyncBodyTypeUnknown;
    mBodyPreferenceTruncationSize = 0;
}

ActiveSyncSyncRequest::ActiveSyncSyncRequest()
{
    init();
}

ActiveSyncSyncRequest::~ActiveSyncSyncRequest()
{
    MC_SAFE_RELEASE(mCollectionID);
    MC_SAFE_RELEASE(mSyncKey);
    MC_SAFE_RELEASE(mCollectionClass);
}

void ActiveSyncSyncRequest::setCollectionID(String * value)
{
    MC_SET_STRING_FIELD(mCollectionID, value);
}

String * ActiveSyncSyncRequest::collectionID()
{
    MC_GET_STRING_FIELD(mCollectionID);
}

void ActiveSyncSyncRequest::setSyncKey(String * value)
{
    MC_SET_STRING_FIELD(mSyncKey, value);
}

String * ActiveSyncSyncRequest::syncKey()
{
    MC_GET_STRING_FIELD(mSyncKey);
}

void ActiveSyncSyncRequest::setCollectionClass(String * value)
{
    MC_SET_STRING_FIELD(mCollectionClass, value);
}

String * ActiveSyncSyncRequest::collectionClass()
{
    MC_GET_STRING_FIELD(mCollectionClass);
}

void ActiveSyncSyncRequest::setGetChanges(bool value)
{
    mGetChanges = value;
}

bool ActiveSyncSyncRequest::getChanges()
{
    return mGetChanges;
}

void ActiveSyncSyncRequest::setDeletesAsMoves(bool value)
{
    mDeletesAsMoves = value;
}

bool ActiveSyncSyncRequest::deletesAsMoves()
{
    return mDeletesAsMoves;
}

void ActiveSyncSyncRequest::setFilterType(ActiveSyncFilterType value)
{
    mHasFilterType = true;
    mFilterType = value;
}

ActiveSyncFilterType ActiveSyncSyncRequest::filterType()
{
    return mFilterType;
}

bool ActiveSyncSyncRequest::hasFilterType()
{
    return mHasFilterType;
}

void ActiveSyncSyncRequest::setConflict(uint32_t value)
{
    mHasConflict = true;
    mConflict = value;
}

uint32_t ActiveSyncSyncRequest::conflict()
{
    return mConflict;
}

bool ActiveSyncSyncRequest::hasConflict()
{
    return mHasConflict;
}

void ActiveSyncSyncRequest::setWindowSize(uint32_t value)
{
    mWindowSize = value;
}

uint32_t ActiveSyncSyncRequest::windowSize()
{
    return mWindowSize;
}

void ActiveSyncSyncRequest::setBodyPreference(ActiveSyncBodyType type, uint32_t truncationSize)
{
    mHasBodyPreference = true;
    mBodyPreferenceType = type;
    mBodyPreferenceTruncationSize = truncationSize;
}

ActiveSyncBodyType ActiveSyncSyncRequest::bodyPreferenceType()
{
    return mBodyPreferenceType;
}

uint32_t ActiveSyncSyncRequest::bodyPreferenceTruncationSize()
{
    return mBodyPreferenceTruncationSize;
}

bool ActiveSyncSyncRequest::hasBodyPreference()
{
    return mHasBodyPreference;
}

Object * ActiveSyncSyncRequest::copy()
{
    ActiveSyncSyncRequest * result = new ActiveSyncSyncRequest();
    result->setCollectionID(collectionID());
    result->setSyncKey(syncKey());
    result->setCollectionClass(collectionClass());
    result->setGetChanges(getChanges());
    result->setDeletesAsMoves(deletesAsMoves());

    if (hasFilterType())
        result->setFilterType(filterType());
    if (hasConflict())
        result->setConflict(conflict());

    result->setWindowSize(windowSize());

    if (hasBodyPreference())
        result->setBodyPreference(bodyPreferenceType(), bodyPreferenceTruncationSize());

    return result;
}

String * ActiveSyncSyncRequest::description()
{
    return String::stringWithUTF8Format("<%s:%p %s>", className()->UTF8Characters(), this, MCUTF8(mCollectionID));
}
