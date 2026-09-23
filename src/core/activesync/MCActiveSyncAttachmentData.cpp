#include "MCActiveSyncAttachmentData.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncAttachmentData::init()
{
    mStatus = ActiveSyncItemOperationsStatusUnknown;
    mFileReference = NULL;
    mRange = NULL;
    mTotal = 0;
    mData = NULL;
}

ActiveSyncAttachmentData::ActiveSyncAttachmentData()
{
    init();
}

ActiveSyncAttachmentData::~ActiveSyncAttachmentData()
{
    MC_SAFE_RELEASE(mFileReference);
    MC_SAFE_RELEASE(mRange);
    MC_SAFE_RELEASE(mData);
}

void ActiveSyncAttachmentData::setStatus(ActiveSyncItemOperationsStatus value)
{
    mStatus = value;
}

ActiveSyncItemOperationsStatus ActiveSyncAttachmentData::status()
{
    return mStatus;
}

void ActiveSyncAttachmentData::setFileReference(String * value)
{
    MC_SET_STRING_FIELD(mFileReference, value);
}

String * ActiveSyncAttachmentData::fileReference()
{
    MC_GET_STRING_FIELD(mFileReference);
}

void ActiveSyncAttachmentData::setRange(String * value)
{
    MC_SET_STRING_FIELD(mRange, value);
}

String * ActiveSyncAttachmentData::range()
{
    MC_GET_STRING_FIELD(mRange);
}

void ActiveSyncAttachmentData::setTotal(uint32_t value)
{
    mTotal = value;
}

uint32_t ActiveSyncAttachmentData::total()
{
    return mTotal;
}

void ActiveSyncAttachmentData::setData(Data * value)
{
    MC_SET_OBJECT_FIELD(Data, mData, value);
}

Data * ActiveSyncAttachmentData::data()
{
    MC_GET_OBJECT_FIELD(mData);
}

Object * ActiveSyncAttachmentData::copy()
{
    ActiveSyncAttachmentData * result = new ActiveSyncAttachmentData();
    result->setStatus(status());
    result->setFileReference(fileReference());
    result->setRange(range());
    result->setTotal(total());
    result->setData(data());
    return result;
}

String * ActiveSyncAttachmentData::description()
{
    return String::stringWithUTF8Format("<%s:%p %s>", className()->UTF8Characters(), this, MCUTF8(mFileReference));
}
