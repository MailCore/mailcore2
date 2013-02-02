#include "MCIMAPMessagePart.h"

using namespace mailcore;

IMAPMessagePart::IMAPMessagePart()
{
}

IMAPMessagePart::IMAPMessagePart(IMAPMessagePart * other) : AbstractMessagePart(other)
{
}

IMAPMessagePart::~IMAPMessagePart()
{
}

Object * IMAPMessagePart::copy()
{
    return new IMAPMessagePart(this);
}
