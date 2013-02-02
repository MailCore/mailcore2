#include "MCIMAPPart.h"

#include <string.h>

#include "MCIMAPMessagePart.h"
#include "MCIMAPMultipart.h"
#include "MCMessageHeader.h"

using namespace mailcore;

void IMAPPart::init()
{
    mPartID = NULL;
	mEncoding = Encoding8Bit;
    mSize = 0;
}

IMAPPart::IMAPPart()
{
    init();
}

IMAPPart::IMAPPart(IMAPPart * other) : AbstractPart(other)
{
    init();
    MC_SAFE_REPLACE_COPY(String, mPartID, other->mPartID);
	mEncoding = other->mEncoding;
    mSize = other->mSize;
}

IMAPPart::~IMAPPart()
{
    MC_SAFE_RELEASE(mPartID);
}

Object * IMAPPart::copy()
{
    return new IMAPPart(this);
}

void IMAPPart::setPartID(String * partID)
{
    MC_SAFE_REPLACE_COPY(String, mPartID, partID);
}

String * IMAPPart::partID()
{
    return mPartID;
}

void IMAPPart::setSize(unsigned int size)
{
    mSize = size;
}

unsigned int IMAPPart::size()
{
    return mSize;
}

void IMAPPart::setEncoding(Encoding encoding)
{
    mEncoding = encoding;
}

Encoding IMAPPart::encoding()
{
    return mEncoding;
}

unsigned int IMAPPart::decodedSize()
{
	switch (mEncoding) {
		case MAILIMAP_BODY_FLD_ENC_BASE64:
            return mSize * 3 / 4;
            
        default:
            return mSize;
	}
}

AbstractPart * IMAPPart::attachmentWithIMAPBody(struct mailimap_body * body)
{
    String * partID;
    
    partID = NULL;
	if (body->bd_type == MAILIMAP_BODY_1PART) {
        partID = MCSTR("1");
    }
    return attachmentWithIMAPBodyInternal(body, partID);
}

AbstractPart * IMAPPart::attachmentWithIMAPBodyInternal(struct mailimap_body * body, String * partID)
{
	switch (body->bd_type) {
		case MAILIMAP_BODY_1PART:
        return attachmentWithIMAPBody1Part(body->bd_data.bd_body_1part, partID);
		case MAILIMAP_BODY_MPART:
        return attachmentWithIMAPBodyMultipart(body->bd_data.bd_body_mpart, partID);
	}
	
    return NULL;
}

AbstractPart * IMAPPart::attachmentWithIMAPBody1Part(struct mailimap_body_type_1part * body_1part,
    String * partID)
{
	switch (body_1part->bd_type) {
		case MAILIMAP_BODY_TYPE_1PART_BASIC:
		{
			IMAPPart * attachment;
            
			attachment = attachmentWithIMAPBody1PartBasic(body_1part->bd_data.bd_type_basic,
                body_1part->bd_ext_1part);
            attachment->setPartID(partID);
            return attachment;
		}
		case MAILIMAP_BODY_TYPE_1PART_MSG:
		{
			return attachmentWithIMAPBody1PartMessage(body_1part->bd_data.bd_type_msg,
                body_1part->bd_ext_1part, partID);
		}
		case MAILIMAP_BODY_TYPE_1PART_TEXT:
		{
			IMAPPart * attachment;
			
			attachment = attachmentWithIMAPBody1PartText(body_1part->bd_data.bd_type_text,
                body_1part->bd_ext_1part);
            attachment->setPartID(partID);
            MCLog("attachment %s", MCUTF8(partID));
            return attachment;
		}
	}
	
	return NULL;
}

IMAPMessagePart * IMAPPart::attachmentWithIMAPBody1PartMessage(struct mailimap_body_type_msg * message,
    struct mailimap_body_ext_1part * extension,
    String * partID)
{
	IMAPMessagePart * attachment;
	AbstractPart * subAttachment;
    String * nextPartID;
    
    nextPartID = NULL;
    if (message->bd_body->bd_type == MAILIMAP_BODY_1PART) {
        // msg or 1part
        nextPartID = partID->stringByAppendingUTF8Format(".1");
    }
    else if (message->bd_body->bd_type == MAILIMAP_BODY_MPART) {
        // mpart
        nextPartID = partID;
    }
	
    attachment = new IMAPMessagePart();
    attachment->header()->importIMAPEnvelope(message->bd_envelope);
    attachment->importIMAPFields(message->bd_fields, extension);

    subAttachment = attachmentWithIMAPBodyInternal(message->bd_body, nextPartID);
    attachment->setMainPart(subAttachment);
    attachment->setMimeType(MCSTR("message/rfc822"));

    return (IMAPMessagePart *) attachment->autorelease();
}

void IMAPPart::importIMAPFields(struct mailimap_body_fields * fields,
    struct mailimap_body_ext_1part * extension)
{
    AbstractPart::importIMAPFields(fields, extension);
    
    setSize(fields->bd_size);
    if (fields->bd_encoding != NULL) {
        bool isUUEncode;
        
        isUUEncode = false;
        if (fields->bd_encoding->enc_type == MAILIMAP_BODY_FLD_ENC_OTHER) {
            if (strcasecmp(fields->bd_encoding->enc_value, "x-uuencode") == 0) {
                isUUEncode = true;
            }
        }
        if (isUUEncode) {
            setEncoding(EncodingUUEncode);
        }
        else {
            setEncoding((Encoding) fields->bd_encoding->enc_type);
        }
    }
}

IMAPPart * IMAPPart::attachmentWithIMAPBody1PartBasic(struct mailimap_body_type_basic * basic,
    struct mailimap_body_ext_1part * extension)
{
	IMAPPart * attachment;
	String * mimeType;
	
	attachment = new IMAPPart();
    attachment->importIMAPFields(basic->bd_fields, extension);
	attachment->setUniqueID(mailcore::String::uuidString());
    
    mimeType = NULL;
	switch (basic->bd_media_basic->med_type) {
		case MAILIMAP_MEDIA_BASIC_APPLICATION:
        mimeType = String::stringWithUTF8Format("application/%s", basic->bd_media_basic->med_subtype);
        break;
		case MAILIMAP_MEDIA_BASIC_AUDIO:
        mimeType = String::stringWithUTF8Format("audio/%s", basic->bd_media_basic->med_subtype);
        break;
		case MAILIMAP_MEDIA_BASIC_IMAGE:
        mimeType = String::stringWithUTF8Format("image/%s", basic->bd_media_basic->med_subtype);
        break;
		case MAILIMAP_MEDIA_BASIC_MESSAGE:
        mimeType = String::stringWithUTF8Format("message/%s", basic->bd_media_basic->med_subtype);
        break;
		case MAILIMAP_MEDIA_BASIC_VIDEO:
        mimeType = String::stringWithUTF8Format("video/%s", basic->bd_media_basic->med_subtype);
        break;
		case MAILIMAP_MEDIA_BASIC_OTHER:
        mimeType = String::stringWithUTF8Format("other/%s", basic->bd_media_basic->med_subtype);
        break;
	}
    attachment->setMimeType(mimeType);
	
    return (IMAPPart *) attachment->autorelease();
}

IMAPPart * IMAPPart::attachmentWithIMAPBody1PartText(struct mailimap_body_type_text * text,
    struct mailimap_body_ext_1part * extension)
{
	IMAPPart * attachment;
	
	attachment = new IMAPPart();
    attachment->importIMAPFields(text->bd_fields, extension);
	attachment->setMimeType(String::stringWithUTF8Format("text/%s", text->bd_media_text));
	
    return (IMAPPart *) attachment->autorelease();
}

IMAPMultipart * IMAPPart::attachmentWithIMAPBodyMultipart(struct mailimap_body_type_mpart * body_mpart,
    String * partID)
{
    clistiter * cur;
    IMAPMultipart * attachment;
    unsigned int count;
    Array * attachments;

    attachments = new Array();

    count = 1;
    for(cur = clist_begin(body_mpart->bd_list) ; cur != NULL ; cur = clist_next(cur)) {
        struct mailimap_body * body;
        AbstractPart * subResult;
        String * nextPartID;

        if (partID == NULL) {
            nextPartID = String::stringWithUTF8Format("%u", count);
        }
        else {
            nextPartID = partID->stringByAppendingUTF8Format(".%u", count);
        }
        body = (struct mailimap_body *) clist_content(cur);
        subResult = attachmentWithIMAPBodyInternal(body, nextPartID);
        attachments->addObject(subResult);

        count ++;
    }

    attachment = new IMAPMultipart();
    if (strcasecmp(body_mpart->bd_media_subtype, "alternative") == 0) {
        attachment->setPartType(PartTypeMultipartAlternative);
    }
    else if (strcasecmp(body_mpart->bd_media_subtype, "related") == 0) {
        attachment->setPartType(PartTypeMultipartRelated);
    }
    attachment->setMimeType(String::stringWithUTF8Format("multipart/%s", body_mpart->bd_media_subtype));
    attachment->setParts(attachments);

    attachments->release();

    return (IMAPMultipart *) attachment->autorelease();
}
