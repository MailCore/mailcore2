#include "MCIMAPMultipart.h"

using namespace mailcore;

IMAPMultipart::IMAPMultipart()
{
}

IMAPMultipart::IMAPMultipart(IMAPMultipart * other) : AbstractMultipart(other)
{
}

IMAPMultipart::~IMAPMultipart()
{
}

#if 0
String * IMAPMultipart::className()
{
    return MCSTR("IMAPMultipart");
}
#endif

Object * IMAPMultipart::copy()
{
    return new IMAPMultipart(this);
}

