#include "MCJMAPSubmission.h"

using namespace mailcore;

void JMAPSubmission::init()
{
    mIdentifier = NULL;
    mEmailID = NULL;
    mThreadID = NULL;
    mIdentityID = NULL;
    mSendAt = NULL;
    mUndoStatus = NULL;
    mDeliveryStatus = (HashMap *) HashMap::hashMap()->retain();
}

JMAPSubmission::JMAPSubmission()
{
    init();
}

JMAPSubmission::~JMAPSubmission()
{
    MC_SAFE_RELEASE(mIdentifier);
    MC_SAFE_RELEASE(mEmailID);
    MC_SAFE_RELEASE(mThreadID);
    MC_SAFE_RELEASE(mIdentityID);
    MC_SAFE_RELEASE(mSendAt);
    MC_SAFE_RELEASE(mUndoStatus);
    MC_SAFE_RELEASE(mDeliveryStatus);
}

String * JMAPSubmission::identifier() { return mIdentifier; }
void JMAPSubmission::setIdentifier(String * identifier) { MC_SAFE_REPLACE_COPY(String, mIdentifier, identifier); }
String * JMAPSubmission::emailID() { return mEmailID; }
void JMAPSubmission::setEmailID(String * emailID) { MC_SAFE_REPLACE_COPY(String, mEmailID, emailID); }
String * JMAPSubmission::threadID() { return mThreadID; }
void JMAPSubmission::setThreadID(String * threadID) { MC_SAFE_REPLACE_COPY(String, mThreadID, threadID); }
String * JMAPSubmission::identityID() { return mIdentityID; }
void JMAPSubmission::setIdentityID(String * identityID) { MC_SAFE_REPLACE_COPY(String, mIdentityID, identityID); }
String * JMAPSubmission::sendAt() { return mSendAt; }
void JMAPSubmission::setSendAt(String * sendAt) { MC_SAFE_REPLACE_COPY(String, mSendAt, sendAt); }
String * JMAPSubmission::undoStatus() { return mUndoStatus; }
void JMAPSubmission::setUndoStatus(String * undoStatus) { MC_SAFE_REPLACE_COPY(String, mUndoStatus, undoStatus); }
HashMap * JMAPSubmission::deliveryStatus() { return mDeliveryStatus; }
void JMAPSubmission::setDeliveryStatus(HashMap * deliveryStatus) { MC_SAFE_REPLACE_COPY(HashMap, mDeliveryStatus, deliveryStatus); }
