#include "MCAbstractPart.h"

#include <string.h>
#include <stdlib.h>
#include <libetpan/libetpan.h>
#include "MCData.h"

using namespace mailcore;

AbstractPart::AbstractPart()
{
    init();
}

AbstractPart::AbstractPart(AbstractPart * other)
{
    init();
    setUniqueID(other->mUniqueID);
    setFilename(other->mFilename);
    setMimeType(other->mMimeType);
    setCharset(other->mCharset);
    setContentID(other->mContentID);
    setContentLocation(other->mContentLocation);
    setInlineAttachment(other->mInlineAttachment);
    setPartType(other->mPartType);
}

void AbstractPart::init()
{
    mUniqueID = NULL;
    mFilename = NULL;
    mMimeType = NULL;
    mCharset = NULL;
    mContentID = NULL;
    mContentLocation = NULL;
    mInlineAttachment = false;
    mPartType = PartTypeSingle;
}

AbstractPart::~AbstractPart()
{
    MC_SAFE_RELEASE(mUniqueID);
    MC_SAFE_RELEASE(mFilename);
    MC_SAFE_RELEASE(mMimeType);
    MC_SAFE_RELEASE(mCharset);
    MC_SAFE_RELEASE(mContentID);
    MC_SAFE_RELEASE(mContentLocation);
}

String * AbstractPart::description()
{
    String * result = String::string();
    result->appendUTF8Format("<%s:%p\n", className()->UTF8Characters(), this);
    if (mFilename != NULL) {
        result->appendUTF8Format("filename: %s\n", mFilename->UTF8Characters());
    }
    if (mMimeType != NULL) {
        result->appendUTF8Format("mime type: %s\n", mMimeType->UTF8Characters());
    }
    if (mCharset != NULL) {
        result->appendUTF8Format("charset: %s\n", mCharset->UTF8Characters());
    }
    if (mContentID != NULL) {
        result->appendUTF8Format("content-ID: %s\n", mContentID->UTF8Characters());
    }
    if (mContentLocation != NULL) {
        result->appendUTF8Format("content-location: %s\n", mContentLocation->UTF8Characters());
    }
    result->appendUTF8Format("inline: %i\n", mInlineAttachment);
    result->appendUTF8Format(">");
    
    return result;
}

Object * AbstractPart::copy()
{
    return new AbstractPart(this);
}

PartType AbstractPart::partType()
{
    return mPartType;
}

void AbstractPart::setPartType(PartType type)
{
    mPartType = type;
}

String * AbstractPart::uniqueID()
{
    return mUniqueID;
}

void AbstractPart::setUniqueID(String * uniqueID)
{
    MC_SAFE_REPLACE_COPY(String, mUniqueID, uniqueID);
}

String * AbstractPart::filename()
{
    return mFilename;
}

void AbstractPart::setFilename(String * filename)
{
    MC_SAFE_REPLACE_COPY(String, mFilename, filename);
}

String * AbstractPart::mimeType()
{
    return mMimeType;
}

void AbstractPart::setMimeType(String * mimeType)
{
    MC_SAFE_REPLACE_COPY(String, mMimeType, mimeType);
}

String * AbstractPart::charset()
{
    return mCharset;
}

void AbstractPart::setCharset(String * charset)
{
    MC_SAFE_REPLACE_COPY(String, mCharset, charset);
}

String * AbstractPart::contentID()
{
    return mContentID;
}

void AbstractPart::setContentID(String * contentID)
{
    MC_SAFE_REPLACE_COPY(String, mContentID, contentID);
}

String * AbstractPart::contentLocation()
{
    return mContentLocation;
}

void AbstractPart::setContentLocation(String * contentLocation)
{
    MC_SAFE_REPLACE_COPY(String, mContentLocation, contentLocation);
}

bool AbstractPart::isInlineAttachment()
{
    return mInlineAttachment;
}

void AbstractPart::setInlineAttachment(bool inlineAttachment)
{
    mInlineAttachment = inlineAttachment;
}

AbstractMessage * AbstractPart::message()
{
    return mMessage;
}

void AbstractPart::setMessage(AbstractMessage * message)
{
    mMessage = message;
}

void AbstractPart::importIMAPFields(struct mailimap_body_fields * fields,
    struct mailimap_body_ext_1part * extension)
{
	if (fields->bd_parameter != NULL) {
		clistiter * cur;
		
		for(cur = clist_begin(fields->bd_parameter->pa_list) ; cur != NULL ;
			cur = clist_next(cur)) {
			struct mailimap_single_body_fld_param * imap_param;
			
			imap_param = (struct mailimap_single_body_fld_param *) clist_content(cur);
			
			if (strcasecmp(imap_param->pa_name, "name") == 0) {
                setFilename(String::stringByDecodingMIMEHeaderValue(imap_param->pa_value));
			}
			else if (strcasecmp(imap_param->pa_name, "charset") == 0) {
                setCharset(String::stringByDecodingMIMEHeaderValue(imap_param->pa_value));
			}
		}
	}
    if (fields->bd_id != NULL) {
		char * contentid;
		size_t cur_token;
		int r;
		
		cur_token = 0;
		r = mailimf_msg_id_parse(fields->bd_id, strlen(fields->bd_id),
            &cur_token, &contentid);
		if (r == MAILIMF_NO_ERROR) {
			// msg id
            setContentID(String::stringWithUTF8Characters(contentid));
            free(contentid);
		}
    }
	
    if (extension != NULL) {
        if (extension->bd_disposition != NULL) {
            if (strcasecmp(extension->bd_disposition->dsp_type, "inline") == 0) {
                setInlineAttachment(true);
            }
            
            if (extension->bd_disposition->dsp_attributes != NULL) {
                clistiter * cur;
                
                for(cur = clist_begin(extension->bd_disposition->dsp_attributes->pa_list) ; cur != NULL ;
                    cur = clist_next(cur)) {
                    struct mailimap_single_body_fld_param * imap_param;
                    
                    imap_param = (struct mailimap_single_body_fld_param *) clist_content(cur);
                    
                    if (strcasecmp(imap_param->pa_name, "filename") == 0) {
                        setFilename(String::stringByDecodingMIMEHeaderValue(imap_param->pa_value));
                    }
                }
            }
        }
        
        if (extension->bd_loc != NULL) {
            setContentLocation(String::stringWithUTF8Characters(extension->bd_loc));
        }
    }
}

AbstractPart * AbstractPart::partForContentID(String * contentID)
{
    if (contentID->isEqual(mContentID)) {
        return this;
    }
    else {
        return NULL;
    }
}

AbstractPart * AbstractPart::partForUniqueID(String * uniqueID)
{
    if (uniqueID->isEqual(mUniqueID)) {
        return this;
    }
    else {
        return NULL;
    }
}

String * AbstractPart::decodedStringForData(Data * data)
{
	String *lowerMimeType = mMimeType ? mMimeType->lowercaseString() : NULL;
	
	if (lowerMimeType && lowerMimeType->hasPrefix(MCSTR("text/"))) {
		bool isHTML = lowerMimeType->isEqual(MCSTR("text/html"));
		return data->stringWithDetectedCharset(mCharset, isHTML);
	}
	else {
		return NULL;
	}
}
