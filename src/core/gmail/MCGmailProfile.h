#ifndef MAILCORE_MCGMAILPROFILE_H

#define MAILCORE_MCGMAILPROFILE_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailProfile : public Object {
    public:
        GmailProfile();
        virtual ~GmailProfile();

        virtual String * emailAddress();
        virtual void setEmailAddress(String * emailAddress);

        virtual uint32_t messagesTotal();
        virtual void setMessagesTotal(uint32_t messagesTotal);

        virtual uint32_t threadsTotal();
        virtual void setThreadsTotal(uint32_t threadsTotal);

        virtual String * historyID();
        virtual void setHistoryID(String * historyID);

        virtual String * description();

    private:
        String * mEmailAddress;
        uint32_t mMessagesTotal;
        uint32_t mThreadsTotal;
        String * mHistoryID;

        void init();
    };

}

#endif

#endif
