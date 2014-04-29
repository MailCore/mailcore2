#include "MCMessagePart.h"
#include "MCMessageHeader.h"

#include <libetpan/libetpan.h>

using namespace mailcore;

MessagePart::MessagePart()
{
}

MessagePart::MessagePart(MessagePart * other) : AbstractMessagePart(other)
{
}

MessagePart::~MessagePart()
{
}

Object * MessagePart::copy()
{
    return new MessagePart(this);
}

struct mailmime * MessagePart::mime() {
    struct mailimf_fields * fields;
    struct mailmime * mime;
    int r;

    fields = header()->createIMFFieldsAndFilterBcc(false);
    mime = mailmime_new_message_data(NULL);
    mailmime_set_imf_fields(mime, fields);

    if (mainPart() != NULL) {
        struct mailmime *mime_sub;

        mime_sub = mainPart()->mime();
        if (mime_sub != NULL) {
            r = mailmime_add_part(mime, mime_sub);
            if (r != MAILIMF_NO_ERROR) {
                mailmime_free(mime);
                mailmime_free(mime_sub);
                return NULL;
            }
        }
    }
    
    return mime;
}