#include "MCJMAPMailbox.h"

using namespace mailcore;

void JMAPMailbox::init()
{
    mIdentifier = NULL;
    mName = NULL;
    mParentID = NULL;
    mRole = NULL;
    mSortOrder = 0;
    mSubscribed = false;
    mTotalEmails = 0;
    mUnreadEmails = 0;
    mTotalThreads = 0;
    mUnreadThreads = 0;
    mRights = (HashMap *) HashMap::hashMap()->retain();
}

JMAPMailbox::JMAPMailbox()
{
    init();
}

JMAPMailbox::~JMAPMailbox()
{
    MC_SAFE_RELEASE(mIdentifier);
    MC_SAFE_RELEASE(mName);
    MC_SAFE_RELEASE(mParentID);
    MC_SAFE_RELEASE(mRole);
    MC_SAFE_RELEASE(mRights);
}

String * JMAPMailbox::identifier() { return mIdentifier; }
void JMAPMailbox::setIdentifier(String * identifier) { MC_SAFE_REPLACE_COPY(String, mIdentifier, identifier); }
String * JMAPMailbox::name() { return mName; }
void JMAPMailbox::setName(String * name) { MC_SAFE_REPLACE_COPY(String, mName, name); }
String * JMAPMailbox::parentID() { return mParentID; }
void JMAPMailbox::setParentID(String * parentID) { MC_SAFE_REPLACE_COPY(String, mParentID, parentID); }
String * JMAPMailbox::role() { return mRole; }
void JMAPMailbox::setRole(String * role) { MC_SAFE_REPLACE_COPY(String, mRole, role); }
int JMAPMailbox::sortOrder() { return mSortOrder; }
void JMAPMailbox::setSortOrder(int sortOrder) { mSortOrder = sortOrder; }
bool JMAPMailbox::isSubscribed() { return mSubscribed; }
void JMAPMailbox::setSubscribed(bool subscribed) { mSubscribed = subscribed; }
int JMAPMailbox::totalEmails() { return mTotalEmails; }
void JMAPMailbox::setTotalEmails(int totalEmails) { mTotalEmails = totalEmails; }
int JMAPMailbox::unreadEmails() { return mUnreadEmails; }
void JMAPMailbox::setUnreadEmails(int unreadEmails) { mUnreadEmails = unreadEmails; }
int JMAPMailbox::totalThreads() { return mTotalThreads; }
void JMAPMailbox::setTotalThreads(int totalThreads) { mTotalThreads = totalThreads; }
int JMAPMailbox::unreadThreads() { return mUnreadThreads; }
void JMAPMailbox::setUnreadThreads(int unreadThreads) { mUnreadThreads = unreadThreads; }
HashMap * JMAPMailbox::rights() { return mRights; }
void JMAPMailbox::setRights(HashMap * rights) { MC_SAFE_REPLACE_COPY(HashMap, mRights, rights); }
