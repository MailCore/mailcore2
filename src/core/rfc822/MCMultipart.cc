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

Object * Multipart::copy()
{
    return new Multipart(this);
}

