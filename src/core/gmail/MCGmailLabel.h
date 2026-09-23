#ifndef MAILCORE_MCGMAILLABEL_H

#define MAILCORE_MCGMAILLABEL_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailLabel : public Object {
    public:
        GmailLabel();
        virtual ~GmailLabel();

        virtual String * identifier();
        virtual void setIdentifier(String * identifier);

        virtual String * name();
        virtual void setName(String * name);

        virtual String * type();
        virtual void setType(String * type);

        virtual String * messageListVisibility();
        virtual void setMessageListVisibility(String * visibility);

        virtual String * labelListVisibility();
        virtual void setLabelListVisibility(String * visibility);

        virtual uint32_t messagesTotal();
        virtual void setMessagesTotal(uint32_t messagesTotal);

        virtual uint32_t messagesUnread();
        virtual void setMessagesUnread(uint32_t messagesUnread);

        virtual uint32_t threadsTotal();
        virtual void setThreadsTotal(uint32_t threadsTotal);

        virtual uint32_t threadsUnread();
        virtual void setThreadsUnread(uint32_t threadsUnread);

        virtual String * description();

    private:
        String * mIdentifier;
        String * mName;
        String * mType;
        String * mMessageListVisibility;
        String * mLabelListVisibility;
        uint32_t mMessagesTotal;
        uint32_t mMessagesUnread;
        uint32_t mThreadsTotal;
        uint32_t mThreadsUnread;

        void init();
    };

}

#endif

#endif
