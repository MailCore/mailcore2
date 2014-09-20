#include "MCAttachment.h"
#include "MCData.h"
#include "MCMultipart.h"
#include "MCMessagePart.h"
#include "MCMessageHeader.h"
#include "MCMessageConstants.h"
#include "MCLog.h"
#include "MCZip.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <libetpan/libetpan.h>

using namespace mailcore;

static char * findBlank(const char * str)
{
    char * p = (char *) str;
    while (!((* p == ' ') || (* p == '\t'))) {
        if (* p == 0)
            return NULL;
        p ++;
    }
    return p;
}

HashMap * Attachment::readMimeTypesFile(String * filename)
{
    HashMap * result = HashMap::hashMap();
    
    char line[512];
    FILE * f = fopen(filename->fileSystemRepresentation(), "r");
    if (f == NULL) {
        return result;
    }
    
    while (fgets(line, sizeof(line), f)) {
        char * p;
        String * mimeType;
        
        if (line[0] == '#') {
            continue;
        }
        
        while ((p = strchr(line, '\r')) != NULL) {
            * p = 0;
        }
        while ((p = strchr(line, '\n')) != NULL) {
            * p = 0;
        }
        
        p = findBlank(line);
        if (p == NULL) {
            continue;
        }
        
        * p = 0;
        p ++;
        mimeType = String::stringWithUTF8Characters(line);
        
        while (1) {
            while ((* p == ' ') || (* p == '\t')) {
                p ++;
            }
            
            char * ext_end = findBlank(p);
            if (ext_end == NULL) {
                String * ext = String::stringWithUTF8Characters(p);
                result->setObjectForKey(ext, mimeType);
                break;
            }
            else {
                * ext_end = 0;
                String * ext = String::stringWithUTF8Characters(p);
                result->setObjectForKey(ext, mimeType);
                p = ext_end + 1;
            }
        }
    }
    
    fclose(f);
    
    return result;
}

String * Attachment::mimeTypeForFilename(String * filename)
{
    static HashMap * mimeTypes = NULL;
    static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&lock);
    if (mimeTypes == NULL) {
        mimeTypes = readMimeTypesFile(MCSTR("/etc/apache2/mime.types"));
        mimeTypes->retain();
    }
    pthread_mutex_unlock(&lock);
    
    String * ext;
    String * result;
    
    ext = filename->pathExtension()->lowercaseString();
    result = (String *) mimeTypes->objectForKey(ext);
    if (result != NULL)
        return result;
    
    if (ext->isEqual(MCSTR("jpg"))) {
        return MCSTR("image/jpeg");
    }
    else if (ext->isEqual(MCSTR("jpeg"))) {
        return MCSTR("image/jpeg");
    }
    else if (ext->isEqual(MCSTR("png"))) {
        return MCSTR("image/png");
    }
    else if (ext->isEqual(MCSTR("gif"))) {
        return MCSTR("image/gif");
    }
    else if (ext->isEqual(MCSTR("html"))) {
        return MCSTR("text/html");
    }
    else if (ext->isEqual(MCSTR("txt"))) {
        return MCSTR("text/plain");
    }
    return NULL;
}

Attachment * Attachment::attachmentWithContentsOfFile(String * filename)
{
    const char * cPath = filename->fileSystemRepresentation();
    struct stat statinfo;
    int r;
    
    r = stat(cPath, &statinfo);
    if (r < 0) {
        return NULL;
    }
    
    if (S_ISDIR(statinfo.st_mode)) {
        String * zipFilename = CreateTemporaryZipFileFromFolder(filename);
        if (zipFilename == NULL) {
            return NULL;
        }
        Attachment * result = attachmentWithContentsOfFile(zipFilename);
        RemoveTemporaryZipFile(zipFilename);
        return result;
    }
    else {
        Data * data = Data::dataWithContentsOfFile(filename);
        return attachmentWithData(filename, data);
    }
}

Attachment * Attachment::attachmentWithData(String * filename, Data * data)
{
    Attachment * attachment;
    String * mimeType;
    
    attachment = new Attachment();
	if (filename) {
    mimeType = Attachment::mimeTypeForFilename(filename);
    if (mimeType != NULL) {
        attachment->setMimeType(mimeType);
    }
        attachment->setFilename(filename->lastPathComponent());
    }
    attachment->setData(data);
    
    return (Attachment *) attachment->autorelease();
}

Attachment * Attachment::attachmentWithHTMLString(String * htmlString)
{
    Data * data;
    Attachment * attachment;
    
    attachment = new Attachment();
    attachment->setInlineAttachment(true);
    attachment->setMimeType(MCSTR("text/html"));
    data = htmlString->dataUsingEncoding("utf-8");
    attachment->setData(data);
    
    return (Attachment *) attachment->autorelease();
}

Attachment * Attachment::attachmentWithRFC822Message(Data * messageData)
{
    Attachment * attachment;
    
    attachment = new Attachment();
    attachment->setMimeType(MCSTR("message/rfc822"));
    attachment->setData(messageData);
    
    return (Attachment *) attachment->autorelease();
}

Attachment * Attachment::attachmentWithText(String * text)
{
    Data * data;
    Attachment * attachment;
    
    attachment = new Attachment();
    attachment->setInlineAttachment(true);
    attachment->setMimeType(MCSTR("text/plain"));
    data = text->dataUsingEncoding("utf-8");
    attachment->setData(data);
    return (Attachment *) attachment->autorelease();
}

void Attachment::init()
{
    mData = NULL;
    mContentTypeParameters = NULL;
    mlcContentTypeParameters = NULL;
    setMimeType(MCSTR("application/octet-stream"));
}

Attachment::Attachment()
{
    init();
}

Attachment::Attachment(Attachment * other) : AbstractPart(other)
{
    init();
    MC_SAFE_REPLACE_RETAIN(Data, mData, other->mData);
    setContentTypeParameters(other->mContentTypeParameters);
}

Attachment::~Attachment()
{
    MC_SAFE_RELEASE(mData);
    MC_SAFE_RELEASE(mContentTypeParameters);
    MC_SAFE_RELEASE(mlcContentTypeParameters);
}

String * Attachment::description()
{
    String * result = String::string();
    result->appendUTF8Format("<%s:%p\n", className()->UTF8Characters(), this);
    if (filename() != NULL) {
        result->appendUTF8Format("filename: %s\n", filename()->UTF8Characters());
    }
    if (mimeType() != NULL) {
        result->appendUTF8Format("mime type: %s\n", mimeType()->UTF8Characters());
    }
    if (charset() != NULL) {
        result->appendUTF8Format("charset: %s\n", charset()->UTF8Characters());
    }
    if (contentID() != NULL) {
        result->appendUTF8Format("content-ID: %s\n", contentID()->UTF8Characters());
    }
    if (contentLocation() != NULL) {
        result->appendUTF8Format("content-location: %s\n", contentLocation()->UTF8Characters());
    }
    result->appendUTF8Format("inline: %i\n", isInlineAttachment());
    if (mData != NULL) {
        result->appendUTF8Format("data: %i bytes\n", mData->length());
    }
    else {
        result->appendUTF8Format("no data\n");
    }
    if (mContentTypeParameters != NULL) {
        mc_foreachhashmapKeyAndValue(String, key, String, value, mContentTypeParameters) {
            result->appendUTF8Format("%s: %s\n", key->UTF8Characters(), value->UTF8Characters());
        }
    }
    result->appendUTF8Format(">");
    
    return result;
}

Object * Attachment::copy()
{
    return new Attachment(this);
}

void Attachment::setData(Data * data)
{
    MC_SAFE_REPLACE_RETAIN(Data, mData, data);
}

Data * Attachment::data()
{
    return mData;
}

String * Attachment::decodedString()
{
    if (mData) {
        return decodedStringForData(mData);
    }
    else {
        return NULL;
    }
}

void Attachment::setContentTypeParameters(HashMap * parameters)
{
    MC_SAFE_REPLACE_COPY(HashMap, mContentTypeParameters, parameters);
    MC_SAFE_RELEASE(mlcContentTypeParameters);
    if (mContentTypeParameters != NULL) {
        mlcContentTypeParameters = new HashMap();
        mc_foreachhashmapKeyAndValue(String, key, String, value, mContentTypeParameters) {
            mlcContentTypeParameters->setObjectForKey(key->lowercaseString(), value);
        }
    }
}

Array * Attachment::allContentTypeParametersNames()
{
    if (mContentTypeParameters == NULL)
        return Array::array();
    return mContentTypeParameters->allKeys();
}

void Attachment::setContentTypeParameter(String * name, String * object)
{
    if (mContentTypeParameters == NULL) {
        mContentTypeParameters = new HashMap();
    }
    if (mlcContentTypeParameters == NULL) {
        mlcContentTypeParameters = new HashMap();
    }
    if (object == NULL) {
        removeContentTypeParameter(name);
        return;
    }
    mContentTypeParameters->setObjectForKey(name, object);
    mlcContentTypeParameters->setObjectForKey(name->lowercaseString(), object);
}

void Attachment::removeContentTypeParameter(String * name)
{
    if (mContentTypeParameters == NULL)
        return;
    mContentTypeParameters->removeObjectForKey(name);
    mlcContentTypeParameters->removeObjectForKey(name);
}

String * Attachment::contentTypeParameterValueForName(String * name)
{
    if (mlcContentTypeParameters == NULL)
        return NULL;
    return (String *) mlcContentTypeParameters->objectForKey(name->lowercaseString());
}

AbstractPart * Attachment::attachmentsWithMIME(struct mailmime * mime)
{
    return attachmentsWithMIMEWithMain(mime, true);
}

void Attachment::fillMultipartSubAttachments(AbstractMultipart * multipart, struct mailmime * mime)
{
    switch (mime->mm_type) {
        case MAILMIME_MULTIPLE:
        {
            clistiter * cur;
            Array * subAttachments = Array::array();
            for(cur = clist_begin(mime->mm_data.mm_multipart.mm_mp_list) ; cur != NULL ; cur = clist_next(cur)) {
                struct mailmime * submime;
                AbstractPart * subAttachment;
                
                submime = (struct mailmime *) clist_content(cur);
                subAttachment = attachmentsWithMIMEWithMain(submime, false);
                subAttachments->addObject(subAttachment);
            }
            
            multipart->setParts(subAttachments);
            break;
        }
    }
}

AbstractPart * Attachment::attachmentsWithMIMEWithMain(struct mailmime * mime, bool isMain)
{
    switch (mime->mm_type) {
        case MAILMIME_SINGLE:
        {
            Attachment * attachment;
            attachment = attachmentWithSingleMIME(mime);
            return attachment;
        }
        case MAILMIME_MULTIPLE:
        {
            if ((mime->mm_content_type != NULL) && (mime->mm_content_type->ct_subtype != NULL) &&
            (strcasecmp(mime->mm_content_type->ct_subtype, "alternative") == 0)) {
                Multipart * attachment;
                attachment = new Multipart();
                attachment->setPartType(PartTypeMultipartAlternative);
                fillMultipartSubAttachments(attachment, mime);
                return (Multipart *) attachment->autorelease();
            }
            else if ((mime->mm_content_type != NULL) && (mime->mm_content_type->ct_subtype != NULL) &&
            (strcasecmp(mime->mm_content_type->ct_subtype, "related") == 0)) {
                Multipart * attachment;
                attachment = new Multipart();
                attachment->setPartType(PartTypeMultipartRelated);
                fillMultipartSubAttachments(attachment, mime);
                return (Multipart *) attachment->autorelease();
            }
            else if ((mime->mm_content_type != NULL) && (mime->mm_content_type->ct_subtype != NULL) &&
                     (strcasecmp(mime->mm_content_type->ct_subtype, "signed") == 0)) {
                Multipart * attachment;
                attachment = new Multipart();
                attachment->setPartType(PartTypeMultipartSigned);
                fillMultipartSubAttachments(attachment, mime);
                return (Multipart *) attachment->autorelease();
            }
            else {
                Multipart * attachment;
                attachment = new Multipart();
                fillMultipartSubAttachments(attachment, mime);
                return (Multipart *) attachment->autorelease();
            }
        }
        case MAILMIME_MESSAGE:
        {
            if (isMain) {
                AbstractPart * attachment;
                attachment = attachmentsWithMIMEWithMain(mime->mm_data.mm_message.mm_msg_mime, false);
                return attachment;
            }
            else {
                MessagePart * messagePart;
                messagePart = attachmentWithMessageMIME(mime);
                return messagePart;
            }
        }
    }
    
    return NULL;
}

Encoding Attachment::encodingForMIMEEncoding(struct mailmime_mechanism * mechanism, int defaultMimeEncoding)
{
    Encoding mimeEncoding = (Encoding) defaultMimeEncoding;
    
    if (mechanism != NULL) {
        mimeEncoding = (Encoding) mechanism->enc_type;
    }
    
    switch ((int) mimeEncoding) {
        default:
        case MAILMIME_MECHANISM_ERROR:
            return EncodingOther;
        case MAILMIME_MECHANISM_7BIT:
            return Encoding7Bit;
        case MAILMIME_MECHANISM_8BIT:
            return Encoding8Bit;
        case MAILMIME_MECHANISM_BINARY:
            return EncodingBinary;
        case MAILMIME_MECHANISM_QUOTED_PRINTABLE:
            return EncodingQuotedPrintable;
        case MAILMIME_MECHANISM_BASE64:
            return EncodingBase64;
        case MAILMIME_MECHANISM_TOKEN:
            if (mechanism == NULL)
                return Encoding8Bit;
            if (mechanism->enc_token == NULL)
                return Encoding8Bit;
            
            if (strcasecmp(mechanism->enc_token, "x-uuencode") == 0) {
                return EncodingUUEncode;
            }
            else {
                return EncodingOther;
            }
    }
}

static const char * get_discrete_type(struct mailmime_discrete_type * discrete_type)
{
    switch (discrete_type->dt_type) {
        case MAILMIME_DISCRETE_TYPE_TEXT:
            return "text";
            
        case MAILMIME_DISCRETE_TYPE_IMAGE:
            return "image";
            
        case MAILMIME_DISCRETE_TYPE_AUDIO:
            return "audio";
            
        case MAILMIME_DISCRETE_TYPE_VIDEO:
            return "video";
            
        case MAILMIME_DISCRETE_TYPE_APPLICATION:
            return "application";
            
        case MAILMIME_DISCRETE_TYPE_EXTENSION:
            return discrete_type->dt_extension;
    }
    
    return NULL;
}

static const char *
get_composite_type(struct mailmime_composite_type * composite_type)
{
    switch (composite_type->ct_type) {
        case MAILMIME_COMPOSITE_TYPE_MESSAGE:
            return "message";
            
        case MAILMIME_COMPOSITE_TYPE_MULTIPART:
            return "multipart";
            
        case MAILMIME_COMPOSITE_TYPE_EXTENSION:
            return composite_type->ct_token;
    }
    
    return NULL;
}

static const char *
get_encoding_type(struct mailmime_mechanism *enc)
{
	switch (enc->enc_type) {
		case MAILMIME_MECHANISM_7BIT:
			return "7bit";
		case MAILMIME_MECHANISM_8BIT:
			return "8bit";
		case MAILMIME_MECHANISM_BINARY:
			return "binary";
		case MAILMIME_MECHANISM_BASE64:
			return "base64";
		case MAILMIME_MECHANISM_QUOTED_PRINTABLE:
			return "quoted-printable";
		case MAILMIME_MECHANISM_TOKEN:
			return "token";
		default:
			return NULL;
	}
}

static char * get_content_type_str(struct mailmime_content * content)
{
    const char * str;
    char * result;
    const char * subtype;
    
    if (content == NULL) {
        return strdup("unknown/unknown");
    }
    
    str = "unknown";
    
    switch (content->ct_type->tp_type) {
        case MAILMIME_TYPE_DISCRETE_TYPE:
            str = get_discrete_type(content->ct_type->tp_data.tp_discrete_type);
            break;
            
        case MAILMIME_TYPE_COMPOSITE_TYPE:
            str = get_composite_type(content->ct_type->tp_data.tp_composite_type);
            break;
    }
    
    if (str == NULL)
        str = "unknown";
    subtype = content->ct_subtype;
    if (subtype == NULL)
        subtype = "unknown";
    
    result = (char *) malloc(strlen(str) + strlen(subtype) + 2);
    strcpy(result, str);
    strcat(result, "/");
    strcat(result, subtype);
    
    return result;
}

Attachment * Attachment::attachmentWithSingleMIME(struct mailmime * mime)
{
    struct mailmime_data * data;
    const char * bytes;
    size_t length;
    Attachment * result;
    struct mailmime_single_fields single_fields;
    char * str;
    char * name;
    char * filename;
    char * content_id;
    char * description;
    char * loc;
    Encoding encoding;
    clist * ct_parameters;
    
    MCAssert(mime->mm_type == MAILMIME_SINGLE);
    
    result = new Attachment();
    result->setUniqueID(mailcore::String::uuidString());
    
    data = mime->mm_data.mm_single;
	if (data->dt_type == MAILMIME_DATA_TEXT) {
    bytes = data->dt_data.dt_text.dt_data;
    length = data->dt_data.dt_text.dt_length;
	} else {
		Data *d = Data::dataWithContentsOfFile(String::stringWithUTF8Characters(data->dt_data.dt_filename));
		bytes = d->bytes();
		length = d->length();
	}
    
    mailmime_single_fields_init(&single_fields, mime->mm_mime_fields, mime->mm_content_type);
    
    encoding = encodingForMIMEEncoding(single_fields.fld_encoding, data->dt_encoding);
    
    Data * mimeData;
    mimeData = Data::dataWithBytes(bytes, (unsigned int) length);
    mimeData = mimeData->decodedDataUsingEncoding(encoding);
    result->setData(mimeData);
    
    str = get_content_type_str(mime->mm_content_type);
    result->setMimeType(String::stringWithUTF8Characters(str));
    free(str);
    
    name = single_fields.fld_content_name;
    filename = single_fields.fld_disposition_filename;
    content_id = single_fields.fld_id;
    description = single_fields.fld_description;
    loc = single_fields.fld_location;
    ct_parameters = single_fields.fld_content->ct_parameters;
    
    if (filename != NULL) {
        result->setFilename(String::stringByDecodingMIMEHeaderValue(filename));
    }
    else if (name != NULL) {
        result->setFilename(String::stringByDecodingMIMEHeaderValue(name));
    }
    if (content_id != NULL) {
        result->setContentID(String::stringWithUTF8Characters(content_id));
    }
    if (description != NULL) {
        result->setContentDescription(String::stringWithUTF8Characters(description));
    }
    if (single_fields.fld_content_charset != NULL) {
        result->setCharset(String::stringByDecodingMIMEHeaderValue(single_fields.fld_content_charset));
    }
    if (loc != NULL) {
        result->setContentLocation(String::stringWithUTF8Characters(loc));
    }
    if (single_fields.fld_encoding) {
		str = (char *)get_encoding_type(single_fields.fld_encoding);
		result->setTransferEncoding(String::stringWithUTF8Characters(str));
	}
    
    if (ct_parameters != NULL) {
        clistiter * iter = clist_begin(ct_parameters);
        struct mailmime_parameter * param;
        while (iter != NULL) {
            param = (struct mailmime_parameter *) clist_content(iter);
            if (param != NULL) {
                result->setContentTypeParameter(String::stringWithUTF8Characters(param->pa_name), String::stringWithUTF8Characters(param->pa_value));
            }
            iter = clist_next(iter);
        }
    }
   
    if (single_fields.fld_disposition != NULL) {
        if (single_fields.fld_disposition->dsp_type != NULL) {
            if (single_fields.fld_disposition->dsp_type->dsp_type == MAILMIME_DISPOSITION_TYPE_INLINE) {
                result->setInlineAttachment(true);
            }
        }
    }
    
    return (Attachment *) result->autorelease();
}

MessagePart * Attachment::attachmentWithMessageMIME(struct mailmime * mime)
{
    MessagePart * attachment;
    AbstractPart * mainPart;
    
    attachment = new MessagePart();
    attachment->header()->importIMFFields(mime->mm_data.mm_message.mm_fields);
    mainPart = attachmentsWithMIMEWithMain(mime->mm_data.mm_message.mm_msg_mime, false);
    attachment->setMainPart(mainPart);
    
    return (MessagePart *) attachment->autorelease();
}

