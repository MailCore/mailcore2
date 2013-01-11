#include "MCIMAPFolder.h"

using namespace mailcore;

void IMAPFolder::init()
{
    mPath = NULL;
    mDelimiter = 0;
    mFlags = IMAPFolderFlagNone;
}

IMAPFolder::IMAPFolder()
{
    init();
}

IMAPFolder::IMAPFolder(IMAPFolder * other)
{
    init();
    setPath(other->path());
    setDelimiter(other->delimiter());
    setFlags(other->flags());
}

IMAPFolder::~IMAPFolder()
{
    MC_SAFE_RELEASE(mPath);
}

#if 0
String * IMAPFolder::className()
{
    return MCSTR("IMAPFolder");
}
#endif

Object * IMAPFolder::copy()
{
    return new IMAPFolder(this);
}

void IMAPFolder::setPath(String * path)
{
    MC_SAFE_REPLACE_COPY(String, mPath, path);
}

String * IMAPFolder::path()
{
    return mPath;
}

void IMAPFolder::setDelimiter(char delimiter)
{
    mDelimiter = delimiter;
}

char IMAPFolder::delimiter()
{
    return mDelimiter;
}

void IMAPFolder::setFlags(IMAPFolderFlag flags)
{
    mFlags = flags;
}

IMAPFolderFlag IMAPFolder::flags()
{
    return mFlags;
}

