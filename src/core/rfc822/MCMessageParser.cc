#include "MCMessageParser.h"

#include <libetpan/libetpan.h>

#include "MCAttachment.h"
#include "MCMessageHeader.h"
#include "MCHTMLRenderer.h"
#include "MCHTMLBodyRendererTemplateCallback.h"

using namespace mailcore;

MessageParser * MessageParser::messageParserWithData(Data * data)
{
    MessageParser * parser = new MessageParser(data);
    return (MessageParser *) parser->autorelease();
}

MessageParser * MessageParser::messageParserWithContentsOfFile(String * filename)
{
    Data * data = Data::dataWithContentsOfFile(filename);
    return messageParserWithData(data);
}

void MessageParser::init()
{
    mData = NULL;
    mMainPart = NULL;
}

MessageParser::MessageParser(Data * data)
{
    init();
    
    const char * start = NULL;
    unsigned int length = 0;
    if (data->length() > 5) {
        if (strncmp(data->bytes(), "From ", 5) == 0) {
            start = data->bytes();
            for(unsigned int i = 0 ; i < data->length() ; i ++) {
                if (start[i] == '\n') {
                    start = start + i + 1;
                    length = data->length() - (i + 1);
                    break;
                }
            }
        }
    }
    if (start != NULL) {
        data = Data::dataWithBytes(start, length);
    }
    
    mData = (Data *) data->retain();
    
    mailmessage * msg;
    struct mailmime * mime;
    
    msg = data_message_init(data->bytes(), data->length());
    mailmessage_get_bodystructure(msg, &mime);
    mMainPart = (AbstractPart *) Attachment::attachmentsWithMIME(msg->msg_mime)->retain();
    mMainPart->applyUniquePartID();
    
    size_t cur_token = 0;
    struct mailimf_fields * fields;
    int r = mailimf_envelope_and_optional_fields_parse(data->bytes(), data->length(), &cur_token, &fields);
    if (r == MAILIMAP_NO_ERROR) {
        header()->importIMFFields(fields);
        mailimf_fields_free(fields);
    }
    mailmessage_free(msg);
}

MessageParser::MessageParser(MessageParser * other) : AbstractMessage(other)
{
    init();
    MC_SAFE_REPLACE_RETAIN(Data, mData, other->mData);
    MC_SAFE_REPLACE_RETAIN(AbstractPart, mMainPart, other->mMainPart);
}

MessageParser::~MessageParser()
{
    MC_SAFE_RELEASE(mMainPart);
    MC_SAFE_RELEASE(mData);
}

AbstractPart * MessageParser::mainPart()
{
    return mMainPart;
}

Data * MessageParser::data()
{
    return mData;
}

String * MessageParser::description()
{
    String * result = String::string();
    result->appendUTF8Format("<%s:%p ", MCUTF8(className()), this);
    result->appendUTF8Format("<%p>", mMainPart);
    result->appendString(mMainPart->description());
    result->appendUTF8Characters(">");
    
    return result;
}

Object * MessageParser::copy()
{
    return new MessageParser(this);
}

AbstractPart * MessageParser::partForContentID(String * contentID)
{
    return mainPart()->partForContentID(contentID);
}

AbstractPart * MessageParser::partForUniqueID(String * uniqueID)
{
    return mainPart()->partForUniqueID(uniqueID);
}

String * MessageParser::htmlRendering(HTMLRendererTemplateCallback * htmlCallback)
{
    return HTMLRenderer::htmlForRFC822Message(this, htmlCallback);
}

String * MessageParser::htmlBodyRendering()
{
    HTMLBodyRendererTemplateCallback * callback = new HTMLBodyRendererTemplateCallback();
    String * result = htmlRendering(callback);
    MC_SAFE_RELEASE(callback);
    return result;
}

String * MessageParser::plainTextRendering()
{
    String * html = htmlRendering(NULL);
    return html->flattenHTML();
}

String * MessageParser::plainTextBodyRendering(bool stripWhitespace)
{
    String * html = htmlBodyRendering();
    String * plainTextBodyString = html->flattenHTML();
    
    if (stripWhitespace) {
        plainTextBodyString = plainTextBodyString->stripWhitespace();
    }
    return plainTextBodyString;
}
