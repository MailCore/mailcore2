#include "MCMessageParser.h"

#include "MCAttachment.h"
#include "MCMessageHeader.h"

using namespace mailcore;

MessageParser * MessageParser::messageParserWithData(Data * data)
{
    MessageParser * parser = new MessageParser(data);
    return (MessageParser *) parser->autorelease();
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
    MCLog("%s:%p ", MCUTF8(mMainPart->className()), mMainPart);
    MCLog("%s:%p ", MCUTF8(mMainPart->description()), mMainPart);
    mMainPart->setMessage(this);
    header()->importIMFFields(msg->msg_fields);
	mailmessage_free(msg);
}

MessageParser::MessageParser(MessageParser * other)
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
