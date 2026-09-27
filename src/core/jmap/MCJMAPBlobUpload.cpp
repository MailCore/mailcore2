#include "MCJMAPBlobUpload.h"

using namespace mailcore;

void JMAPBlobUpload::init()
{
    mAccountID = NULL;
    mBlobID = NULL;
    mType = NULL;
    mName = NULL;
    mSize = 0;
}

JMAPBlobUpload::JMAPBlobUpload()
{
    init();
}

JMAPBlobUpload::~JMAPBlobUpload()
{
    MC_SAFE_RELEASE(mAccountID);
    MC_SAFE_RELEASE(mBlobID);
    MC_SAFE_RELEASE(mType);
    MC_SAFE_RELEASE(mName);
}

String * JMAPBlobUpload::accountID() { return mAccountID; }
void JMAPBlobUpload::setAccountID(String * accountID) { MC_SAFE_REPLACE_COPY(String, mAccountID, accountID); }
String * JMAPBlobUpload::blobID() { return mBlobID; }
void JMAPBlobUpload::setBlobID(String * blobID) { MC_SAFE_REPLACE_COPY(String, mBlobID, blobID); }
String * JMAPBlobUpload::type() { return mType; }
void JMAPBlobUpload::setType(String * type) { MC_SAFE_REPLACE_COPY(String, mType, type); }
String * JMAPBlobUpload::name() { return mName; }
void JMAPBlobUpload::setName(String * name) { MC_SAFE_REPLACE_COPY(String, mName, name); }
size_t JMAPBlobUpload::size() { return mSize; }
void JMAPBlobUpload::setSize(size_t size) { mSize = size; }
