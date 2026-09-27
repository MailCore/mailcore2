#include "MCJMAPMessagePart.h"

using namespace mailcore;

void JMAPMessagePart::init()
{
    mPartID = NULL;
    mBlobID = NULL;
    mSize = 0;
}

JMAPMessagePart::JMAPMessagePart()
{
    init();
}

JMAPMessagePart::~JMAPMessagePart()
{
    MC_SAFE_RELEASE(mPartID);
    MC_SAFE_RELEASE(mBlobID);
}

String * JMAPMessagePart::partID() { return mPartID; }
void JMAPMessagePart::setPartID(String * partID) { MC_SAFE_REPLACE_COPY(String, mPartID, partID); }
String * JMAPMessagePart::blobID() { return mBlobID; }
void JMAPMessagePart::setBlobID(String * blobID) { MC_SAFE_REPLACE_COPY(String, mBlobID, blobID); }
unsigned int JMAPMessagePart::size() { return mSize; }
void JMAPMessagePart::setSize(unsigned int size) { mSize = size; }
