#include "MCMultipart.h"

#include "MCDefines.h"

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

Object * Multipart::copy()
{
    return new Multipart(this);
}

static void * createObject()
{
    return new Multipart();
}

INITIALIZE(Multipart)
{
    Object::registerObjectConstructor("mailcore::Multipart", &createObject);
}
