#ifndef MAILCORE_MCGMAILMESSAGEDATAOPERATION_H

#define MAILCORE_MCGMAILMESSAGEDATAOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailMessageDataOperation : public GmailOperation {
    public:
        GmailMessageDataOperation();
        virtual ~GmailMessageDataOperation();

        virtual void setMessageID(String * messageID);
        virtual String * messageID();
        virtual Data * data();
        virtual void main();

    private:
        String * mMessageID;
        Data * mData;
    };

}

#endif

#endif
