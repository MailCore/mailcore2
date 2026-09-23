#include "MCActiveSyncBodyPart.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncBodyPart::init()
{
    mStatus = ActiveSyncItemOperationsStatusUnknown;
    mType = ActiveSyncBodyTypeUnknown;
    mData = NULL;
    mEstimatedDataSize = 0;
    mTruncated = false;
    mPreview = NULL;
}

ActiveSyncBodyPart::ActiveSyncBodyPart()
{
    init();
}

ActiveSyncBodyPart::~ActiveSyncBodyPart()
{
    MC_SAFE_RELEASE(mData);
    MC_SAFE_RELEASE(mPreview);
}

void ActiveSyncBodyPart::setStatus(ActiveSyncItemOperationsStatus value)
{
    mStatus = value;
}

ActiveSyncItemOperationsStatus ActiveSyncBodyPart::status()
{
    return mStatus;
}

void ActiveSyncBodyPart::setType(ActiveSyncBodyType value)
{
    mType = value;
}

ActiveSyncBodyType ActiveSyncBodyPart::type()
{
    return mType;
}

void ActiveSyncBodyPart::setData(Data * value)
{
    MC_SET_OBJECT_FIELD(Data, mData, value);
}

Data * ActiveSyncBodyPart::data()
{
    MC_GET_OBJECT_FIELD(mData);
}

void ActiveSyncBodyPart::setEstimatedDataSize(uint32_t value)
{
    mEstimatedDataSize = value;
}

uint32_t ActiveSyncBodyPart::estimatedDataSize()
{
    return mEstimatedDataSize;
}

void ActiveSyncBodyPart::setTruncated(bool value)
{
    mTruncated = value;
}

bool ActiveSyncBodyPart::isTruncated()
{
    return mTruncated;
}

void ActiveSyncBodyPart::setPreview(String * value)
{
    MC_SET_STRING_FIELD(mPreview, value);
}

String * ActiveSyncBodyPart::preview()
{
    MC_GET_STRING_FIELD(mPreview);
}

Object * ActiveSyncBodyPart::copy()
{
    ActiveSyncBodyPart * result = new ActiveSyncBodyPart();
    result->setStatus(status());
    result->setType(type());
    result->setData(data());
    result->setEstimatedDataSize(estimatedDataSize());
    result->setTruncated(isTruncated());
    result->setPreview(preview());
    return result;
}

String * ActiveSyncBodyPart::description()
{
    return String::stringWithUTF8Format("<%s:%p type:%i>", className()->UTF8Characters(), this, type());
}
