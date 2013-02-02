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

Object * MessagePart::copy()
{
    return new MessagePart(this);
}
