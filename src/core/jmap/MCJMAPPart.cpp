#include "MCJMAPPart.h"

using namespace mailcore;

void JMAPPart::init()
{
    mPartID = NULL;
    mBlobID = NULL;
    mSize = 0;
    mLanguage = NULL;
    mLanguages = (Array *) Array::array()->retain();
}

JMAPPart::JMAPPart()
{
    init();
}

JMAPPart::~JMAPPart()
{
    MC_SAFE_RELEASE(mPartID);
    MC_SAFE_RELEASE(mBlobID);
    MC_SAFE_RELEASE(mLanguage);
    MC_SAFE_RELEASE(mLanguages);
}

String * JMAPPart::partID() { return mPartID; }
void JMAPPart::setPartID(String * partID) { MC_SAFE_REPLACE_COPY(String, mPartID, partID); }
String * JMAPPart::blobID() { return mBlobID; }
void JMAPPart::setBlobID(String * blobID) { MC_SAFE_REPLACE_COPY(String, mBlobID, blobID); }
unsigned int JMAPPart::size() { return mSize; }
void JMAPPart::setSize(unsigned int size) { mSize = size; }
String * JMAPPart::language() { return mLanguage; }
void JMAPPart::setLanguage(String * language) { MC_SAFE_REPLACE_COPY(String, mLanguage, language); }
Array * JMAPPart::languages() { return mLanguages; }
void JMAPPart::setLanguages(Array * languages) { MC_SAFE_REPLACE_COPY(Array, mLanguages, languages); }
