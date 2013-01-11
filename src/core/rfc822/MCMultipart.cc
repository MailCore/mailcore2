#include "MCMultipart.h"

using namespace mailcore;

Multipart::Multipart()
{
}

Multipart::Multipart(Multipart * other) : AbstractMultipart(other)
{
}

Multipart::~Multipart()
{
}

#if 0
String * Multipart::className()
{
    return MCSTR("Multipart");
}
#endif

Object * Multipart::copy()
{
    return new Multipart(this);
}

