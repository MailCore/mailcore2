#ifndef MAILCORE_MCGMAILMESSAGEPARTDATAOPERATION_H

#define MAILCORE_MCGMAILMESSAGEPARTDATAOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class GmailMessagePart;

    class MAILCORE_EXPORT GmailMessagePartDataOperation : public GmailOperation {
    public:
        GmailMessagePartDataOperation();
        virtual ~GmailMessagePartDataOperation();

        virtual void setMessageID(String * messageID);
        virtual String * messageID();
        virtual void setPart(GmailMessagePart * part);
        virtual GmailMessagePart * part();
        virtual Data * data();
        virtual void main();

    private:
        String * mMessageID;
        GmailMessagePart * mPart;
        Data * mData;
    };

}

#endif

#endif
