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
    mData = (Data *) data->retain();
    
	mailmessage * msg;
	struct mailmime * mime;
	
	msg = data_message_init(data->bytes(), data->length());
	mailmessage_get_bodystructure(msg, &mime);
    mMainPart = (AbstractPart *) Attachment::attachmentsWithMIME(msg->msg_mime)->retain();
    mMainPart->applyUniquePartID();
    header()->importIMFFields(msg->msg_fields);
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

String * MessageParser::plainTextBodyRendering()
{
    String * html = htmlBodyRendering();
    String * plainTextBodyString = html->flattenHTML();
    
    plainTextBodyString->replaceOccurrencesOfString(MCSTR("\t"), MCSTR(" "));
    plainTextBodyString->replaceOccurrencesOfString(MCSTR("\n"), MCSTR(" "));
    plainTextBodyString->replaceOccurrencesOfString(MCSTR("\v"), MCSTR(" "));
    plainTextBodyString->replaceOccurrencesOfString(MCSTR("\f"), MCSTR(" "));
    plainTextBodyString->replaceOccurrencesOfString(MCSTR("\r"), MCSTR(" "));
    while (plainTextBodyString->replaceOccurrencesOfString(MCSTR("  "), MCSTR(" "))) {
        // do nothing.
    }
    return plainTextBodyString;
}
