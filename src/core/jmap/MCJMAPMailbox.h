#ifndef MAILCORE_MCJMAPMAILBOX_H

#define MAILCORE_MCJMAPMAILBOX_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT JMAPMailbox : public Object {
    public:
        JMAPMailbox();
        virtual ~JMAPMailbox();

        virtual String * identifier();
        virtual void setIdentifier(String * identifier);

        virtual String * name();
        virtual void setName(String * name);

        virtual String * parentID();
        virtual void setParentID(String * parentID);

        virtual String * role();
        virtual void setRole(String * role);

        virtual int sortOrder();
        virtual void setSortOrder(int sortOrder);

        virtual bool isSubscribed();
        virtual void setSubscribed(bool subscribed);

        virtual int totalEmails();
        virtual void setTotalEmails(int totalEmails);

        virtual int unreadEmails();
        virtual void setUnreadEmails(int unreadEmails);

        virtual int totalThreads();
        virtual void setTotalThreads(int totalThreads);

        virtual int unreadThreads();
        virtual void setUnreadThreads(int unreadThreads);

        virtual HashMap * rights();
        virtual void setRights(HashMap * rights);

    private:
        String * mIdentifier;
        String * mName;
        String * mParentID;
        String * mRole;
        int mSortOrder;
        bool mSubscribed;
        int mTotalEmails;
        int mUnreadEmails;
        int mTotalThreads;
        int mUnreadThreads;
        HashMap * mRights;

        void init();
    };

}

#endif

#endif
