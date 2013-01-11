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

#if 0
String * IMAPMessagePart::className()
{
    return MCSTR("IMAPMessagePart");
}
#endif

Object * IMAPMessagePart::copy()
{
    return new IMAPMessagePart(this);
}
