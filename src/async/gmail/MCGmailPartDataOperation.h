#ifndef MAILCORE_MCGMAILPARTDATAOPERATION_H

#define MAILCORE_MCGMAILPARTDATAOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class AbstractPart;

    class MAILCORE_EXPORT GmailPartDataOperation : public GmailOperation {
    public:
        GmailPartDataOperation();
        virtual ~GmailPartDataOperation();

        virtual void setMessageID(String * messageID);
        virtual String * messageID();
        virtual void setPart(AbstractPart * part);
        virtual AbstractPart * part();
        virtual Data * data();
        virtual void main();

    private:
        String * mMessageID;
        AbstractPart * mPart;
        Data * mData;
    };

}

#endif

#endif
