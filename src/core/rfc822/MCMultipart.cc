#include "MCMultipart.h"
#include <libetpan/libetpan.h>

static char * generate_boundary(const char * boundary_prefix);

using namespace mailcore;

Multipart::Multipart()
{
    init();
}

Multipart::Multipart(Multipart * other) : AbstractMultipart(other)
{
    init();
    MC_SAFE_REPLACE_COPY(String, mBoundaryPrefix, other->mBoundaryPrefix);
}

Multipart::~Multipart()
{
    MC_SAFE_RELEASE(mBoundaryPrefix);
}

Object * Multipart::copy()
{
    return new Multipart(this);
}

void Multipart::init() {
    mBoundaryPrefix = NULL;
}

void Multipart::setBoundaryPrefix(String *boundaryPrefix) {
    MC_SAFE_REPLACE_COPY(String, mBoundaryPrefix, boundaryPrefix);
}

String * Multipart::boundaryPrefix() {
    return mBoundaryPrefix;
}

struct mailmime * Multipart::mime()
{
    struct mailmime_fields * mime_fields;
    struct mailmime_content * content;
    struct mailmime * mime;
    int r;
    int i;
    int mime_type;
    clist * list;
    char * attr_name;
    char * attr_value;
    struct mailmime_parameter * param;
    clist * parameters;
    char * boundary;

    mime_fields = mailmime_fields_new_empty();
    if (mime_fields == NULL) {
        goto err;
    }

    content = mailmime_content_new_with_str(MCUTF8(mimeType()));
    if (content == NULL) {
        goto free_fields;
    }

    mime_type = MAILMIME_MULTIPLE;

    list = clist_new();
    if (list == NULL)
        goto free_content;
    
    attr_name = strdup("boundary");
    if (attr_name == NULL)
        goto free_list;
    
    boundary = generate_boundary(MCUTF8(boundaryPrefix()));
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
    {
        parameters = content->ct_parameters;
    }
    
    r = clist_append(parameters, param);
    if (r != 0) {
        clist_free(parameters);
        mailmime_parameter_free(param);
        goto free_list;
    }
    
    if (content->ct_parameters == NULL)
    {
        content->ct_parameters = parameters;
    }

    mime = mailmime_new(mime_type, NULL, 0, mime_fields, content, NULL, NULL, NULL, list, NULL, NULL);

    for (i = 0; i < parts()->count(); i ++) {
        AbstractPart * subPart;
        struct mailmime * mime_sub;

        subPart = (AbstractPart *)parts()->objectAtIndex(i);
        mime_sub = subPart->mime();

        if (mime_sub) {
            mailmime_add_part(mime, mime_sub);
        }
    }

    return mime;

free_list:
    clist_free(list);
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