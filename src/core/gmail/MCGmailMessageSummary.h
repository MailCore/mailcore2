#ifndef MAILCORE_MCGMAILMESSAGESUMMARY_H

#define MAILCORE_MCGMAILMESSAGESUMMARY_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailMessageSummary : public Object {
    public:
        GmailMessageSummary();
        virtual ~GmailMessageSummary();

        virtual String * identifier();
        virtual void setIdentifier(String * identifier);

        virtual String * threadID();
        virtual void setThreadID(String * threadID);

    private:
        String * mIdentifier;
        String * mThreadID;

        void init();
    };

}

#endif

#endif
