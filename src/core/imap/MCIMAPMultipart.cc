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

Object * IMAPMultipart::copy()
{
    return new IMAPMultipart(this);
}

