#include "MCActiveSyncBody.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncBody::init()
{
    mType = ActiveSyncBodyTypeUnknown;
    mData = NULL;
    mEstimatedDataSize = 0;
    mTruncated = false;
    mNativeBodyType = ActiveSyncBodyTypeUnknown;
    mMimeType = NULL;
    mPreview = NULL;
    mAttachments = NULL;
}

ActiveSyncBody::ActiveSyncBody()
{
    init();
}

ActiveSyncBody::~ActiveSyncBody()
{
    MC_SAFE_RELEASE(mData);
    MC_SAFE_RELEASE(mMimeType);
    MC_SAFE_RELEASE(mPreview);
    MC_SAFE_RELEASE(mAttachments);
}

void ActiveSyncBody::setType(ActiveSyncBodyType value)
{
    mType = value;
}

ActiveSyncBodyType ActiveSyncBody::type()
{
    return mType;
}

void ActiveSyncBody::setData(Data * value)
{
    MC_SET_OBJECT_FIELD(Data, mData, value);
}

Data * ActiveSyncBody::data()
{
    MC_GET_OBJECT_FIELD(mData);
}

void ActiveSyncBody::setEstimatedDataSize(uint32_t value)
{
    mEstimatedDataSize = value;
}

uint32_t ActiveSyncBody::estimatedDataSize()
{
    return mEstimatedDataSize;
}

void ActiveSyncBody::setTruncated(bool value)
{
    mTruncated = value;
}

bool ActiveSyncBody::isTruncated()
{
    return mTruncated;
}

void ActiveSyncBody::setNativeBodyType(ActiveSyncBodyType value)
{
    mNativeBodyType = value;
}

ActiveSyncBodyType ActiveSyncBody::nativeBodyType()
{
    return mNativeBodyType;
}

void ActiveSyncBody::setMimeType(String * value)
{
    MC_SET_STRING_FIELD(mMimeType, value);
}

String * ActiveSyncBody::mimeType()
{
    MC_GET_STRING_FIELD(mMimeType);
}

void ActiveSyncBody::setPreview(String * value)
{
    MC_SET_STRING_FIELD(mPreview, value);
}

String * ActiveSyncBody::preview()
{
    MC_GET_STRING_FIELD(mPreview);
}

void ActiveSyncBody::setAttachments(Array * /* ActiveSyncAttachment */ value)
{
    MC_SET_OBJECT_FIELD(Array, mAttachments, value);
}

Array * /* ActiveSyncAttachment */ ActiveSyncBody::attachments()
{
    MC_GET_OBJECT_FIELD(mAttachments);
}

Object * ActiveSyncBody::copy()
{
    ActiveSyncBody * result = new ActiveSyncBody();
    result->setType(type());
    result->setData(data());
    result->setEstimatedDataSize(estimatedDataSize());
    result->setTruncated(isTruncated());
    result->setNativeBodyType(nativeBodyType());
    result->setMimeType(mimeType());
    result->setPreview(preview());
    result->setAttachments(attachments());
    return result;
}

String * ActiveSyncBody::description()
{
    return String::stringWithUTF8Format("<%s:%p type:%i>", className()->UTF8Characters(), this, type());
}
