#include "MCJMAPMultipart.h"

using namespace mailcore;

void JMAPMultipart::init()
{
    mPartID = NULL;
    mBlobID = NULL;
    mSize = 0;
    mLanguage = NULL;
    mLanguages = (Array *) Array::array()->retain();
}

JMAPMultipart::JMAPMultipart()
{
    init();
}

JMAPMultipart::~JMAPMultipart()
{
    MC_SAFE_RELEASE(mPartID);
    MC_SAFE_RELEASE(mBlobID);
    MC_SAFE_RELEASE(mLanguage);
    MC_SAFE_RELEASE(mLanguages);
}

String * JMAPMultipart::partID() { return mPartID; }
void JMAPMultipart::setPartID(String * partID) { MC_SAFE_REPLACE_COPY(String, mPartID, partID); }
String * JMAPMultipart::blobID() { return mBlobID; }
void JMAPMultipart::setBlobID(String * blobID) { MC_SAFE_REPLACE_COPY(String, mBlobID, blobID); }
unsigned int JMAPMultipart::size() { return mSize; }
void JMAPMultipart::setSize(unsigned int size) { mSize = size; }
String * JMAPMultipart::language() { return mLanguage; }
void JMAPMultipart::setLanguage(String * language) { MC_SAFE_REPLACE_COPY(String, mLanguage, language); }
Array * JMAPMultipart::languages() { return mLanguages; }
void JMAPMultipart::setLanguages(Array * languages) { MC_SAFE_REPLACE_COPY(Array, mLanguages, languages); }
