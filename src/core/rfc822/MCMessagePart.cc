#include "MCMessagePart.h"

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

#if 0
String * MessagePart::className()
{
    return MCSTR("MessagePart");
}
#endif

Object * MessagePart::copy()
{
    return new MessagePart(this);
}
