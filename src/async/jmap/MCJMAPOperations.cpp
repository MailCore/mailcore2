#include "MCJMAPOperations.h"

#include "MCJMAPAsyncSession.h"
#include "MCJMAP.h"

using namespace mailcore;

JMAPFetchMailboxesOperation::JMAPFetchMailboxesOperation() { mMailboxes = NULL; }
JMAPFetchMailboxesOperation::~JMAPFetchMailboxesOperation() { MC_SAFE_RELEASE(mMailboxes); }
Array * JMAPFetchMailboxesOperation::mailboxes() { return mMailboxes; }
void JMAPFetchMailboxesOperation::main()
{
    ErrorCode error;
    Array * mailboxes = session()->session()->fetchMailboxes(&error);
    MC_SAFE_REPLACE_RETAIN(Array, mMailboxes, mailboxes);
    setError(error);
}

JMAPQueryMessagesOperation::JMAPQueryMessagesOperation()
{
    mText = NULL;
    mMailboxID = NULL;
    mPosition = 0;
    mLimit = 50;
    mMessageIDs = NULL;
}

JMAPQueryMessagesOperation::~JMAPQueryMessagesOperation()
{
    MC_SAFE_RELEASE(mText);
    MC_SAFE_RELEASE(mMailboxID);
    MC_SAFE_RELEASE(mMessageIDs);
}

void JMAPQueryMessagesOperation::setText(String * text) { MC_SAFE_REPLACE_COPY(String, mText, text); }
String * JMAPQueryMessagesOperation::text() { return mText; }
void JMAPQueryMessagesOperation::setMailboxID(String * mailboxID) { MC_SAFE_REPLACE_COPY(String, mMailboxID, mailboxID); }
String * JMAPQueryMessagesOperation::mailboxID() { return mMailboxID; }
void JMAPQueryMessagesOperation::setPosition(unsigned int position) { mPosition = position; }
unsigned int JMAPQueryMessagesOperation::position() { return mPosition; }
void JMAPQueryMessagesOperation::setLimit(unsigned int limit) { mLimit = limit; }
unsigned int JMAPQueryMessagesOperation::limit() { return mLimit; }
Array * JMAPQueryMessagesOperation::messageIDs() { return mMessageIDs; }
void JMAPQueryMessagesOperation::main()
{
    ErrorCode error;
    Array * ids = NULL;
    if (mMailboxID != NULL)
        ids = session()->session()->queryMessagesInMailbox(mMailboxID, mPosition, mLimit, &error);
    else
        ids = session()->session()->queryMessagesWithText(mText, mPosition, mLimit, &error);
    MC_SAFE_REPLACE_RETAIN(Array, mMessageIDs, ids);
    setError(error);
}

JMAPFetchMessagesOperation::JMAPFetchMessagesOperation()
{
    mMessageIDs = NULL;
    mProperties = NULL;
    mMessages = NULL;
}

JMAPFetchMessagesOperation::~JMAPFetchMessagesOperation()
{
    MC_SAFE_RELEASE(mMessageIDs);
    MC_SAFE_RELEASE(mProperties);
    MC_SAFE_RELEASE(mMessages);
}

void JMAPFetchMessagesOperation::setMessageIDs(Array * messageIDs) { MC_SAFE_REPLACE_COPY(Array, mMessageIDs, messageIDs); }
Array * JMAPFetchMessagesOperation::messageIDs() { return mMessageIDs; }
void JMAPFetchMessagesOperation::setProperties(Array * properties) { MC_SAFE_REPLACE_COPY(Array, mProperties, properties); }
Array * JMAPFetchMessagesOperation::properties() { return mProperties; }
Array * JMAPFetchMessagesOperation::messages() { return mMessages; }
void JMAPFetchMessagesOperation::main()
{
    ErrorCode error;
    Array * messages = session()->session()->fetchMessages(mMessageIDs, mProperties, &error);
    MC_SAFE_REPLACE_RETAIN(Array, mMessages, messages);
    setError(error);
}

JMAPUploadOperation::JMAPUploadOperation()
{
    mData = NULL;
    mContentType = NULL;
    mAccountID = NULL;
    mUpload = NULL;
}

JMAPUploadOperation::~JMAPUploadOperation()
{
    MC_SAFE_RELEASE(mData);
    MC_SAFE_RELEASE(mContentType);
    MC_SAFE_RELEASE(mAccountID);
    MC_SAFE_RELEASE(mUpload);
}

void JMAPUploadOperation::setData(Data * data) { MC_SAFE_REPLACE_RETAIN(Data, mData, data); }
Data * JMAPUploadOperation::data() { return mData; }
void JMAPUploadOperation::setContentType(String * contentType) { MC_SAFE_REPLACE_COPY(String, mContentType, contentType); }
String * JMAPUploadOperation::contentType() { return mContentType; }
void JMAPUploadOperation::setAccountID(String * accountID) { MC_SAFE_REPLACE_COPY(String, mAccountID, accountID); }
String * JMAPUploadOperation::accountID() { return mAccountID; }
JMAPBlobUpload * JMAPUploadOperation::upload() { return mUpload; }
void JMAPUploadOperation::main()
{
    ErrorCode error;
    JMAPBlobUpload * upload = session()->session()->upload(mData, mContentType, mAccountID, &error);
    MC_SAFE_REPLACE_RETAIN(JMAPBlobUpload, mUpload, upload);
    setError(error);
}

JMAPDownloadOperation::JMAPDownloadOperation()
{
    mBlobID = NULL;
    mName = NULL;
    mAccept = NULL;
    mAccountID = NULL;
    mData = NULL;
}

JMAPDownloadOperation::~JMAPDownloadOperation()
{
    MC_SAFE_RELEASE(mBlobID);
    MC_SAFE_RELEASE(mName);
    MC_SAFE_RELEASE(mAccept);
    MC_SAFE_RELEASE(mAccountID);
    MC_SAFE_RELEASE(mData);
}

void JMAPDownloadOperation::setBlobID(String * blobID) { MC_SAFE_REPLACE_COPY(String, mBlobID, blobID); }
String * JMAPDownloadOperation::blobID() { return mBlobID; }
void JMAPDownloadOperation::setName(String * name) { MC_SAFE_REPLACE_COPY(String, mName, name); }
String * JMAPDownloadOperation::name() { return mName; }
void JMAPDownloadOperation::setAccept(String * accept) { MC_SAFE_REPLACE_COPY(String, mAccept, accept); }
String * JMAPDownloadOperation::accept() { return mAccept; }
void JMAPDownloadOperation::setAccountID(String * accountID) { MC_SAFE_REPLACE_COPY(String, mAccountID, accountID); }
String * JMAPDownloadOperation::accountID() { return mAccountID; }
Data * JMAPDownloadOperation::data() { return mData; }
void JMAPDownloadOperation::main()
{
    ErrorCode error;
    Data * data = session()->session()->download(mBlobID, mName, mAccept, mAccountID, &error);
    MC_SAFE_REPLACE_RETAIN(Data, mData, data);
    setError(error);
}

JMAPCreateDraftOperation::JMAPCreateDraftOperation()
{
    mData = NULL;
    mMailboxID = NULL;
    mKeywords = NULL;
    mMessage = NULL;
}

JMAPCreateDraftOperation::~JMAPCreateDraftOperation()
{
    MC_SAFE_RELEASE(mData);
    MC_SAFE_RELEASE(mMailboxID);
    MC_SAFE_RELEASE(mKeywords);
    MC_SAFE_RELEASE(mMessage);
}

void JMAPCreateDraftOperation::setData(Data * data) { MC_SAFE_REPLACE_RETAIN(Data, mData, data); }
Data * JMAPCreateDraftOperation::data() { return mData; }
void JMAPCreateDraftOperation::setMailboxID(String * mailboxID) { MC_SAFE_REPLACE_COPY(String, mMailboxID, mailboxID); }
String * JMAPCreateDraftOperation::mailboxID() { return mMailboxID; }
void JMAPCreateDraftOperation::setKeywords(Array * keywords) { MC_SAFE_REPLACE_COPY(Array, mKeywords, keywords); }
Array * JMAPCreateDraftOperation::keywords() { return mKeywords; }
JMAPMessage * JMAPCreateDraftOperation::message() { return mMessage; }
void JMAPCreateDraftOperation::main()
{
    ErrorCode error;
    JMAPMessage * message = session()->session()->createDraft(mData, mMailboxID, mKeywords, &error);
    MC_SAFE_REPLACE_RETAIN(JMAPMessage, mMessage, message);
    setError(error);
}

JMAPSendOperation::JMAPSendOperation()
{
    mData = NULL;
    mMessageID = NULL;
    mIdentityID = NULL;
    mSubmission = NULL;
}

JMAPSendOperation::~JMAPSendOperation()
{
    MC_SAFE_RELEASE(mData);
    MC_SAFE_RELEASE(mMessageID);
    MC_SAFE_RELEASE(mIdentityID);
    MC_SAFE_RELEASE(mSubmission);
}

void JMAPSendOperation::setData(Data * data) { MC_SAFE_REPLACE_RETAIN(Data, mData, data); }
Data * JMAPSendOperation::data() { return mData; }
void JMAPSendOperation::setMessageID(String * messageID) { MC_SAFE_REPLACE_COPY(String, mMessageID, messageID); }
String * JMAPSendOperation::messageID() { return mMessageID; }
void JMAPSendOperation::setIdentityID(String * identityID) { MC_SAFE_REPLACE_COPY(String, mIdentityID, identityID); }
String * JMAPSendOperation::identityID() { return mIdentityID; }
JMAPSubmission * JMAPSendOperation::submission() { return mSubmission; }
void JMAPSendOperation::main()
{
    ErrorCode error;
    JMAPSubmission * submission = NULL;
    if (mData != NULL)
        submission = session()->session()->sendData(mData, mIdentityID, &error);
    else
        submission = session()->session()->sendMessage(mMessageID, mIdentityID, &error);
    MC_SAFE_REPLACE_RETAIN(JMAPSubmission, mSubmission, submission);
    setError(error);
}
