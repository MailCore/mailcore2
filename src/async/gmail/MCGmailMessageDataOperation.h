#ifndef MAILCORE_MCGMAILMESSAGEDATAOPERATION_H

#define MAILCORE_MCGMAILMESSAGEDATAOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailMessageDataOperation : public GmailOperation {
        friend class GmailAsyncSession;

    public:
        GmailMessageDataOperation();
        virtual ~GmailMessageDataOperation();

        virtual String * messageID();
        virtual Data * data();
        virtual void main();

    private:
        virtual void setMessageID(String * messageID);

        String * mMessageID;
        Data * mData;
    };

}

#endif

#endif
