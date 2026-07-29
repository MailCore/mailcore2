#include "MCActiveSyncAttachment.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncAttachment::init()
{
    mMethod = ActiveSyncAttachmentMethodUnknown;
    mEstimatedDataSize = 0;
}

ActiveSyncAttachment::ActiveSyncAttachment()
{
    init();
}

ActiveSyncAttachment::~ActiveSyncAttachment()
{
}

void ActiveSyncAttachment::setMethod(ActiveSyncAttachmentMethod value)
{
    mMethod = value;
}

ActiveSyncAttachmentMethod ActiveSyncAttachment::method()
{
    return mMethod;
}

void ActiveSyncAttachment::setEstimatedDataSize(uint32_t value)
{
    mEstimatedDataSize = value;
}

uint32_t ActiveSyncAttachment::estimatedDataSize()
{
    return mEstimatedDataSize;
}

Object * ActiveSyncAttachment::copy()
{
    ActiveSyncAttachment * result = new ActiveSyncAttachment();
    result->setFilename(filename());
    result->setMethod(method());
    result->setContentID(contentID());
    result->setContentLocation(contentLocation());
    result->setInlineAttachment(isInlineAttachment());
    result->setAttachment(isAttachment());
    result->setMimeType(mimeType());
    result->setUniqueID(uniqueID());
    result->setEstimatedDataSize(estimatedDataSize());
    return result;
}

String * ActiveSyncAttachment::description()
{
    return String::stringWithUTF8Format("<%s:%p %s>", className()->UTF8Characters(), this, MCUTF8(filename()));
}
