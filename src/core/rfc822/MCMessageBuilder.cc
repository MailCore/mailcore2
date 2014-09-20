#include "MCMessageBuilder.h"

#include "MCMessageHeader.h"
#include "MCAttachment.h"
#include "MCMessageParser.h"
#include "MCString.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libetpan/libetpan.h>
#include <libetpan/clist.h>
#include <libetpan/mailmime_disposition.h>

using namespace mailcore;

static char * generate_boundary(const char * boundary_prefix);
struct mailmime * part_multiple_new(const char * type, const char * boundary_prefix);
static struct mailmime *
part_new_empty(struct mailmime_content * content,
               struct mailmime_fields * mime_fields,
               const char * boundary_prefix,
               int force_single);

static struct mailmime * get_multipart_alternative(const char * boundary_prefix)
{
    struct mailmime * mime;
    
    mime = part_multiple_new("multipart/alternative", boundary_prefix);
    
    return mime;
}

static struct mailmime * get_multipart_related(const char * boundary_prefix)
{
    struct mailmime * mime;
    
    mime = part_multiple_new("multipart/related", boundary_prefix);
    
    return mime;
}

static int add_attachment(struct mailmime * mime,
    struct mailmime * mime_sub,
    const char *explicit_content,
    const char * boundary_prefix)
{
    struct mailmime * saved_sub;
    struct mailmime * mp;
    int res;
    int r;
    
    switch (mime->mm_type) {
        case MAILMIME_SINGLE:
            res = MAILIMF_ERROR_INVAL;
            goto err;
            
        case MAILMIME_MULTIPLE:
            r = mailmime_add_part(mime, mime_sub);
            if (r != MAILIMF_NO_ERROR) {
                res = MAILIMF_ERROR_MEMORY;
                goto err;
            }
            
            return MAILIMF_NO_ERROR;
    }
    
    /* MAILMIME_MESSAGE */
    
    if (mime->mm_data.mm_message.mm_msg_mime == NULL) {
        /* there is no subpart, we can simply attach it */
        
        r = mailmime_add_part(mime, mime_sub);
        if (r != MAILIMF_NO_ERROR) {
            res = MAILIMF_ERROR_MEMORY;
            goto err;
        }
        
        return MAILIMF_NO_ERROR;
    }
    
    if (mime->mm_data.mm_message.mm_msg_mime->mm_type == MAILMIME_MULTIPLE &&
        strcasecmp(mime->mm_data.mm_message.mm_msg_mime->mm_content_type->ct_subtype, "alternative") != 0) {
        /* in case the subpart is multipart, simply attach it to the subpart */
        
        return mailmime_add_part(mime->mm_data.mm_message.mm_msg_mime, mime_sub);
    }
    
    /* we save the current subpart, ... */
    
    saved_sub = mime->mm_data.mm_message.mm_msg_mime;
    
    /* create a multipart */
    if (explicit_content)
		mp = part_multiple_new(explicit_content, boundary_prefix);
	else
    mp = part_multiple_new("multipart/mixed", boundary_prefix);
	
    if (mp == NULL) {
        res = MAILIMF_ERROR_MEMORY;
        goto err;
    }
    
    /* detach the saved subpart from the parent */
    
    mailmime_remove_part(saved_sub);
    
    /* the created multipart is the new child of the parent */
    
    r = mailmime_add_part(mime, mp);
    if (r != MAILIMF_NO_ERROR) {
        res = MAILIMF_ERROR_MEMORY;
        goto free_mp;
    }
    
    /* then, attach the saved subpart and ... */
    
    r = mailmime_add_part(mp, saved_sub);
    if (r != MAILIMF_NO_ERROR) {
        res = MAILIMF_ERROR_MEMORY;
        goto free_saved_sub;
    }
    
    /* the given part to the parent */
    
    r = mailmime_add_part(mp, mime_sub);
    if (r != MAILIMF_NO_ERROR) {
        res = MAILIMF_ERROR_MEMORY;
        goto free_saved_sub;
    }
    
    return MAILIMF_NO_ERROR;
    
free_mp:
    mailmime_free(mp);
free_saved_sub:
    mailmime_free(saved_sub);
err:
    return res;
}

static struct mailmime * get_text_part(const char * mime_type, const char * charset, const char * content_id,
                                const char * description,
                                const char * text, size_t length, int encoding_type, clist * contentTypeParameters)
{
    struct mailmime_fields * mime_fields;
    struct mailmime * mime;
    struct mailmime_content * content;
    struct mailmime_parameter * param;
    struct mailmime_disposition * disposition;
    struct mailmime_mechanism * encoding;
    char * dup_content_id;
    char * dup_description;
    
    encoding = mailmime_mechanism_new(encoding_type, NULL);
    disposition = mailmime_disposition_new_with_data(MAILMIME_DISPOSITION_TYPE_INLINE,
                                                     NULL, NULL, NULL, NULL, (size_t) -1);
    dup_content_id = NULL;
    if (content_id != NULL)
        dup_content_id = strdup(content_id);
    dup_description = NULL;
    if (dup_description != NULL)
        dup_description = strdup(description);
    mime_fields = mailmime_fields_new_with_data(encoding,
                                                dup_content_id, dup_description, disposition, NULL);
    
    content = mailmime_content_new_with_str(mime_type);
    if (charset == NULL) {
        param = mailmime_param_new_with_data((char *) "charset", (char *) "utf-8");
    }
    else {
        param = mailmime_param_new_with_data((char *) "charset", (char *) charset);
    }
    clist_append(content->ct_parameters, param);
    if (contentTypeParameters != NULL) {
        clist_concat(content->ct_parameters, contentTypeParameters);
    }
    
    mime = part_new_empty(content, mime_fields, NULL, 1);
    mailmime_set_body_text(mime, (char *) text, length);
    
    return mime;
}

static struct mailmime * get_plain_text_part(const char * mime_type, const char * charset, const char * content_id,
                                      const char * description,
                                      const char * text, size_t length, clist * contentTypeParameters)
{
    bool needsQuotedPrintable;
    int mechanism;
    
    needsQuotedPrintable = false;
    for(size_t i = 0 ; i < length ; i ++) {
        if ((text[i] & (1 << 7)) != 0) {
            needsQuotedPrintable = true;
        }
    }
    
    mechanism = MAILMIME_MECHANISM_7BIT;
    if (needsQuotedPrintable) {
        mechanism = MAILMIME_MECHANISM_QUOTED_PRINTABLE;
    }
    return get_text_part(mime_type, charset, content_id, description, text, length, mechanism, contentTypeParameters);
}

static struct mailmime * get_other_text_part(const char * mime_type, const char * charset, const char * content_id,
                                      const char * description,
                                      const char * text, size_t length, clist * contentTypeParameters)
{
    return get_text_part(mime_type, charset, content_id, description, text, length, MAILMIME_MECHANISM_QUOTED_PRINTABLE, contentTypeParameters);
}

static struct mailmime * get_file_part(const char * filename, const char * mime_type, int is_inline,
                                       const char * content_id,
                                       const char * content_description,
									   int explicit_encoding,
									   const char *explicit_disposition,
                                       const char * text, size_t length,
                                       clist * contentTypeParameters)
{
    char * disposition_name;
    int encoding_type;
    struct mailmime_disposition * disposition = NULL;
    struct mailmime_mechanism * encoding;
    struct mailmime_content * content;
    struct mailmime * mime;
    struct mailmime_fields * mime_fields;
    char * dup_content_id;
    char * dup_content_description;

    disposition_name = NULL;
    if (filename != NULL) {
        disposition_name = strdup(filename);
    }
	if (explicit_disposition != NULL) {
		size_t idx = 0;
		int r = mailmime_disposition_parse(explicit_disposition, strlen(explicit_disposition),
										   &idx, &disposition);
		if (r != 0)
			disposition = NULL;

	}
	if (disposition == NULL) {
    if (is_inline) {
        disposition = mailmime_disposition_new_with_data(MAILMIME_DISPOSITION_TYPE_INLINE,
                                                         disposition_name, NULL, NULL, NULL, (size_t) -1);
    }
    else {
        disposition = mailmime_disposition_new_with_data(MAILMIME_DISPOSITION_TYPE_ATTACHMENT,
                                                         disposition_name, NULL, NULL, NULL, (size_t) -1);
    }
	}
    content = mailmime_content_new_with_str(mime_type);
    if (explicit_encoding > 0)
		encoding_type = explicit_encoding;
	else
    encoding_type = MAILMIME_MECHANISM_BASE64;
    encoding = mailmime_mechanism_new(encoding_type, NULL);
    dup_content_id = NULL;
    if (content_id != NULL)
        dup_content_id = strdup(content_id);
    dup_content_description = NULL;
    if (content_description != NULL)
        dup_content_description = strdup(content_description);
    mime_fields = mailmime_fields_new_with_data(encoding,
                                                dup_content_id, dup_content_description, disposition, NULL);
    
    if (contentTypeParameters != NULL) {
        clist_concat(content->ct_parameters, contentTypeParameters);
    }
    
    mime = part_new_empty(content, mime_fields, NULL, 1);
    mailmime_set_body_text(mime, (char *) text, length);
    
    return mime;
}

#define MIME_ENCODED_STR(str) (str != NULL ? str->encodedMIMEHeaderValue()->bytes() : NULL)
static clist * content_type_parameters_from_attachment(Attachment * att)
{
    clist * contentTypeParameters = NULL;
    struct mailmime_parameter * param;
    
    mc_foreacharray(String, name, att->allContentTypeParametersNames()) {
        if (contentTypeParameters == NULL) {
            contentTypeParameters = clist_new();
        }
        String * value = att->contentTypeParameterValueForName(name);
        param = mailmime_param_new_with_data((char *)name->UTF8Characters(), (char *)value->UTF8Characters());
        clist_append(contentTypeParameters, param);
    }

    return contentTypeParameters;
}

static struct mailmime * mime_from_attachment(Attachment * att)
{
    struct mailmime * mime;
    Data * data;
    int r;

    data = att->data();
    if (data == NULL) {
        data = Data::data();
    }
	int explicit_encoding = -1;
	char *explicit_disp = NULL;
	
	if (att->transferEncoding()) {
		int type = MAILMIME_MECHANISM_ERROR;
		
		if (att->transferEncoding()->lowercaseString()->isEqual(MCSTR("7bit"))) {
			type = MAILMIME_MECHANISM_7BIT;
		} else if (att->transferEncoding()->lowercaseString()->isEqual(MCSTR("8bit"))) {
			type = MAILMIME_MECHANISM_8BIT;
		} else if (att->transferEncoding()->lowercaseString()->isEqual(MCSTR("binary"))) {
			type = MAILMIME_MECHANISM_BINARY;
		} else if (att->transferEncoding()->lowercaseString()->isEqual(MCSTR("quoted-printable"))) {
			type = MAILMIME_MECHANISM_QUOTED_PRINTABLE;
		} else if (att->transferEncoding()->lowercaseString()->isEqual(MCSTR("base64"))) {
			type = MAILMIME_MECHANISM_BASE64;
		} /* TODO: TOKEN */
		explicit_encoding = type;
	}
	if (att->disposition() != NULL) {
		/* override the content-disposition */
		explicit_disp = (char *)att->disposition()->UTF8Characters();
	}

    if (att->mimeType()->lowercaseString()->isEqual(MCSTR("message/rfc822"))) {
        size_t indx = 0;
        r = mailmime_parse(data->bytes(), data->length(), &indx, &mime);
        if (r != MAILIMF_NO_ERROR)
            return NULL;
    }
    else {
        clist * contentTypeParameters = content_type_parameters_from_attachment(att);
        if (att->isInlineAttachment() && att->mimeType()->lowercaseString()->isEqual(MCSTR("text/plain"))) {
			if (explicit_encoding != -1)
				mime = get_text_part(MCUTF8(att->mimeType()), MCUTF8(att->charset()),
                                       MCUTF8(att->contentID()),
                                       MIME_ENCODED_STR(att->contentDescription()),
									 data->bytes(), data->length(), explicit_encoding,
                                       contentTypeParameters);
			else
				mime = get_plain_text_part(MCUTF8(att->mimeType()), MCUTF8(att->charset()),
										   MCUTF8(att->contentID()),
										   MIME_ENCODED_STR(att->contentDescription()),
										   data->bytes(), data->length(), contentTypeParameters);
        }
        else if (att->isInlineAttachment() && att->mimeType()->lowercaseString()->hasPrefix(MCSTR("text/"))) {
            mime = get_other_text_part(MCUTF8(att->mimeType()), MCUTF8(att->charset()),
                                       MCUTF8(att->contentID()),
                                       MIME_ENCODED_STR(att->contentDescription()),
            data->bytes(), data->length(), contentTypeParameters);
		} else {
            mime = get_file_part(MIME_ENCODED_STR(att->filename()),
                                 MCUTF8(att->mimeType()), att->isInlineAttachment(),
                                 MCUTF8(att->contentID()),
                                 MIME_ENCODED_STR(att->contentDescription()),
								 explicit_encoding,
								 explicit_disp,
                                 data->bytes(), data->length(),
                                 contentTypeParameters);
        }
    }
    return mime;
}

static struct mailmime * multipart_related_from_attachments(Attachment * htmlAttachment,
    Array * attachments, const char * boundary_prefix)
{
    if ((attachments != NULL) && (attachments->count() > 0)) {
        struct mailmime * submime;
        struct mailmime * mime;
        
        mime = get_multipart_related(boundary_prefix);
        
        submime = mime_from_attachment(htmlAttachment);
        add_attachment(mime, submime, NULL, boundary_prefix);
        
        for(unsigned int i = 0 ; i < attachments->count() ; i ++) {
            Attachment * attachment;
            
            attachment = (Attachment *) attachments->objectAtIndex(i);
            submime = mime_from_attachment(attachment);
            add_attachment(mime, submime, NULL, boundary_prefix);
        }
        
        return mime;
    }
    else {
        struct mailmime * mime;
        
        mime = mime_from_attachment(htmlAttachment);
        
        return mime;
    }
}

static struct mailmime *
part_new_empty(struct mailmime_content * content,
               struct mailmime_fields * mime_fields,
               const char * boundary_prefix,
               int force_single)
{
    struct mailmime * build_info;
    clist * list;
    int r;
    int mime_type;
    
    list = NULL;
    
    if (force_single) {
        mime_type = MAILMIME_SINGLE;
    }
    else {
        switch (content->ct_type->tp_type) {
            case MAILMIME_TYPE_DISCRETE_TYPE:
                mime_type = MAILMIME_SINGLE;
                break;
                
            case MAILMIME_TYPE_COMPOSITE_TYPE:
                switch (content->ct_type->tp_data.tp_composite_type->ct_type) {
                    case MAILMIME_COMPOSITE_TYPE_MULTIPART:
                        mime_type = MAILMIME_MULTIPLE;
                        break;
                        
                    case MAILMIME_COMPOSITE_TYPE_MESSAGE:
                        if (strcasecmp(content->ct_subtype, "rfc822") == 0)
                            mime_type = MAILMIME_MESSAGE;
                        else
                            mime_type = MAILMIME_SINGLE;
                        break;
                        
                    default:
                        goto err;
                }
                break;
                
            default:
                goto err;
        }
    }
    
    if (mime_type == MAILMIME_MULTIPLE) {
        char * attr_name;
        char * attr_value;
        struct mailmime_parameter * param;
        clist * parameters;
        char * boundary;
        
        list = clist_new();
        if (list == NULL)
            goto err;
        
        attr_name = strdup("boundary");
        if (attr_name == NULL)
            goto free_list;
        
        boundary = generate_boundary(boundary_prefix);
        attr_value = boundary;
        if (attr_name == NULL) {
            free(attr_name);
            goto free_list;
        }
        
        param = mailmime_parameter_new(attr_name, attr_value);
        if (param == NULL) {
            free(attr_value);
            free(attr_name);
            goto free_list;
        }
        
        if (content->ct_parameters == NULL) {
            parameters = clist_new();
            if (parameters == NULL) {
                mailmime_parameter_free(param);
                goto free_list;
            }
        }
        else
            parameters = content->ct_parameters;
        
        r = clist_append(parameters, param);
        if (r != 0) {
            clist_free(parameters);
            mailmime_parameter_free(param);
            goto free_list;
        }
        
        if (content->ct_parameters == NULL)
            content->ct_parameters = parameters;
    }
    
    build_info = mailmime_new(mime_type,
                              NULL, 0, mime_fields, content,
                              NULL, NULL, NULL, list,
                              NULL, NULL);
    if (build_info == NULL) {
        clist_free(list);
        return NULL;
    }
    
    return build_info;
    
free_list:
    clist_free(list);
err:
    return NULL;
}

struct mailmime * part_multiple_new(const char * type, const char * boundary_prefix)
{
    struct mailmime_fields * mime_fields;
    struct mailmime_content * content;
    struct mailmime * mp;
    
    mime_fields = mailmime_fields_new_empty();
    if (mime_fields == NULL)
        goto err;
    
    content = mailmime_content_new_with_str(type);
    if (content == NULL)
        goto free_fields;
    
    mp = part_new_empty(content, mime_fields, boundary_prefix, 0);
    if (mp == NULL)
        goto free_content;
    
    return mp;
    
free_content:
    mailmime_content_free(content);
free_fields:
    mailmime_fields_free(mime_fields);
err:
    return NULL;
}

#define MAX_MESSAGE_ID 512

static char * generate_boundary(const char * boundary_prefix)
{
    char id[MAX_MESSAGE_ID];
    time_t now;
    char name[MAX_MESSAGE_ID];
    long value;
    
    now = time(NULL);
    value = random();
    
    gethostname(name, MAX_MESSAGE_ID);
    
    if (boundary_prefix == NULL)
        boundary_prefix = "";
    
    snprintf(id, MAX_MESSAGE_ID, "%s%lx_%lx_%x", boundary_prefix, now, value, getpid());
    
    return strdup(id);
}

void MessageBuilder::init()
{
    mHTMLBody = NULL;
    mTextBody = NULL;
    mAttachments = NULL;
    mRelatedAttachments = NULL;
    mBoundaryPrefix = NULL;
	mContentType = NULL;
}
    
MessageBuilder::MessageBuilder()
{
    init();
}

MessageBuilder::MessageBuilder(MessageBuilder * other) : AbstractMessage(other)
{
    init();
    setHTMLBody(other->mHTMLBody);
    setTextBody(other->mTextBody);
    setAttachments(other->mAttachments);
    setRelatedAttachments(other->mRelatedAttachments);
    MC_SAFE_REPLACE_COPY(String, mBoundaryPrefix, other->mBoundaryPrefix);
	MC_SAFE_REPLACE_COPY(String, mContentType, other->mContentType);
}

MessageBuilder::~MessageBuilder()
{
    MC_SAFE_RELEASE(mHTMLBody);
    MC_SAFE_RELEASE(mTextBody);
    MC_SAFE_RELEASE(mAttachments);
    MC_SAFE_RELEASE(mRelatedAttachments);
    MC_SAFE_RELEASE(mBoundaryPrefix);
	MC_SAFE_RELEASE(mContentType);
}
    
String * MessageBuilder::description()
{
    String * result = String::string();
    result->appendUTF8Format("<%s:%p\n", className()->UTF8Characters(), this);
    if (header() != NULL) {
        result->appendString(header()->description());
        result->appendUTF8Characters("\n");
    }
    if (mHTMLBody != NULL) {
        result->appendUTF8Characters("-- html body --\n");
        result->appendString(mHTMLBody);
        result->appendUTF8Characters("\n");
    }
    if (mTextBody != NULL) {
        result->appendUTF8Characters("-- text body --\n");
        result->appendString(mTextBody);
        result->appendUTF8Characters("\n");
    }
    if (mAttachments != NULL) {
        result->appendUTF8Characters("-- attachments --\n");
        result->appendString(mAttachments->description());
        result->appendUTF8Characters("\n");
    }
    if (mRelatedAttachments != NULL) {
        result->appendUTF8Characters("-- related attachments --\n");
        result->appendString(mRelatedAttachments->description());
        result->appendUTF8Characters("\n");
    }
    result->appendUTF8Characters(">");
    
    return result;
}

Object * MessageBuilder::copy()
{
    return new MessageBuilder(this);
}

void MessageBuilder::setHTMLBody(String * htmlBody)
{
    MC_SAFE_REPLACE_COPY(String, mHTMLBody, htmlBody);
}

String * MessageBuilder::htmlBody()
{
    return mHTMLBody;
}

void MessageBuilder::setTextBody(String * textBody)
{
    MC_SAFE_REPLACE_COPY(String, mTextBody, textBody);
}

String * MessageBuilder::textBody()
{
    return mTextBody;
}

void MessageBuilder::setAttachments(Array * attachments)
{
    MC_SAFE_REPLACE_COPY(Array, mAttachments, attachments);
}

Array * MessageBuilder::attachments()
{
    return mAttachments;
}

void MessageBuilder::addAttachment(Attachment * attachment)
{
    if (attachment == NULL) {
        return;
    }
    if (mAttachments == NULL) {
        mAttachments = new Array();
    }
    mAttachments->addObject(attachment);
}

void MessageBuilder::setRelatedAttachments(Array * attachments)
{
    MC_SAFE_REPLACE_COPY(Array, mRelatedAttachments, attachments);
}

Array * MessageBuilder::relatedAttachments()
{
    return mRelatedAttachments;
}

void MessageBuilder::addRelatedAttachment(Attachment * attachment)
{
    if (attachment == NULL) {
        return;
    }
    if (mRelatedAttachments == NULL) {
        mRelatedAttachments = new Array();
    }
    mRelatedAttachments->addObject(attachment);
}

void MessageBuilder::setBoundaryPrefix(String * boundaryPrefix)
{
    MC_SAFE_REPLACE_COPY(String, mBoundaryPrefix, boundaryPrefix);
}

String * MessageBuilder::boundaryPrefix()
{
    return mBoundaryPrefix;
}

void MessageBuilder::setContentType(String * mpContentType)
{
    MC_SAFE_REPLACE_COPY(String, mContentType, mpContentType);
}

String * MessageBuilder::contentType()
{
    return mContentType;
}

String *MessageBuilder::renderAttachment(Attachment *attachment)
{
	struct mailmime * mime;
    MMAPString * str;
    int col;
	String *mimestr;
	
	mime = mime_from_attachment(attachment);
	
	str = mmap_string_new("");
    col = 0;
	mmap_string_append (str, "Content-Type: ");
	mailmime_content_type_write_mem(str, &col, mime->mm_content_type);
	mmap_string_append (str, "\r\n");
	mailmime_fields_write_mem(str, &col, mime->mm_mime_fields);
	mmap_string_append (str, "\r\n");
    mailmime_write_mem(str, &col, mime);
    Data *data = Data::dataWithBytes(str->str, (unsigned int) str->len);
	
    mmap_string_free(str);
    mailmime_free(mime);

	mimestr = String::stringWithData(data);

	return mimestr;
}

String *MessageBuilder::renderBody()
{
	struct mailmime * mime;
    MMAPString * str;
    int col, i;
	Data *data;

    mime = mailmime_new_message_data(NULL);
	
	char *ctstr = NULL;
	if (mContentType)
		ctstr = (char *)mContentType->UTF8Characters();
	
    if (attachments() != NULL) {
        for(i = 0 ; i < attachments()->count() ; i ++) {
            Attachment * attachment;
            struct mailmime * submime;
			
            attachment = (Attachment *) attachments()->objectAtIndex(i);
            submime = mime_from_attachment(attachment);
            add_attachment(mime, submime, ctstr, MCUTF8(mBoundaryPrefix));
        }
    }
    
    str = mmap_string_new("");
    col = 0;
    mailmime_write_mem(str, &col, mime);
    data = Data::dataWithBytes(str->str, (unsigned int) str->len);
    mmap_string_free(str);
    mailmime_free(mime);
	
	return String::stringWithData(data);
}

Data * MessageBuilder::dataAndFilterBcc(bool filterBcc)
{
    Data * data;
    MMAPString * str;
    int col;

    struct mailmime * htmlPart;
    struct mailmime * textPart;
    struct mailmime * altPart;
    struct mailmime * mainPart;

    htmlPart = NULL;
    textPart = NULL;
    altPart = NULL;
    mainPart = NULL;

    if (htmlBody() != NULL) {
        Attachment * htmlAttachment;

        htmlAttachment = Attachment::attachmentWithHTMLString(htmlBody());
        htmlPart = multipart_related_from_attachments(htmlAttachment, mRelatedAttachments,
            MCUTF8(mBoundaryPrefix));
    }

    if (textBody() != NULL) {
        Attachment * textAttachment;

        textAttachment = Attachment::attachmentWithText(textBody());
        textPart = mime_from_attachment(textAttachment);
    }
    else if (htmlBody() != NULL) {
        Attachment * textAttachment;

        textAttachment = Attachment::attachmentWithText(htmlBody()->flattenHTML());
        textPart = mime_from_attachment(textAttachment);
    }

    if ((textPart != NULL) && (htmlPart != NULL)) {
        altPart = get_multipart_alternative(MCUTF8(mBoundaryPrefix));
        mailmime_smart_add_part(altPart, textPart);
        mailmime_smart_add_part(altPart, htmlPart);
        mainPart = altPart;
    }
    else if (textPart != NULL) {
        mainPart = textPart;
    }
    else if (htmlPart != NULL) {
        mainPart = htmlPart;
    }

    struct mailimf_fields * fields;
    unsigned int i;
    struct mailmime * mime;

    fields = header()->createIMFFieldsAndFilterBcc(filterBcc);

    mime = mailmime_new_message_data(NULL);
    mailmime_set_imf_fields(mime, fields);

	char *ctstr = NULL;
	if (mContentType)
		ctstr = (char *)mContentType->UTF8Characters();
	
    if (mainPart != NULL) {
        add_attachment(mime, mainPart, NULL, MCUTF8(mBoundaryPrefix));
    }

    if (attachments() != NULL) {
        for(i = 0 ; i < attachments()->count() ; i ++) {
            Attachment * attachment;
            struct mailmime * submime;

            attachment = (Attachment *) attachments()->objectAtIndex(i);
            submime = mime_from_attachment(attachment);
            add_attachment(mime, submime, ctstr, MCUTF8(mBoundaryPrefix));
        }
    }
    
    str = mmap_string_new("");
    col = 0;
    mailmime_write_mem(str, &col, mime);
    data = Data::dataWithBytes(str->str, (unsigned int) str->len);
    mmap_string_free(str);
    mailmime_free(mime);

    return data;
}

Data * MessageBuilder::data()
{
    return dataAndFilterBcc(false);
}

String * MessageBuilder::htmlRendering(HTMLRendererTemplateCallback * htmlCallback)
{
    MessageParser * message = MessageParser::messageParserWithData(data());
    return message->htmlRendering(htmlCallback);
}

String * MessageBuilder::htmlBodyRendering()
{
    MessageParser * message = MessageParser::messageParserWithData(data());
    return message->htmlBodyRendering();
}

String * MessageBuilder::plainTextRendering()
{
    MessageParser * message = MessageParser::messageParserWithData(data());
    return message->plainTextRendering();
}

String * MessageBuilder::plainTextBodyRendering(bool stripWhitespace)
{
    MessageParser * message = MessageParser::messageParserWithData(data());
    return message->plainTextBodyRendering(stripWhitespace);
}

