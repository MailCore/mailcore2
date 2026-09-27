#ifndef MAILCORE_MCGMAILPARTDATAOPERATION_H

#define MAILCORE_MCGMAILPARTDATAOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class AbstractPart;

    class MAILCORE_EXPORT GmailPartDataOperation : public GmailOperation {
        friend class GmailAsyncSession;

    public:
        GmailPartDataOperation();
        virtual ~GmailPartDataOperation();

        virtual String * messageID();
        virtual AbstractPart * part();
        virtual Data * data();
        virtual void main();

    private:
        virtual void setMessageID(String * messageID);
        virtual void setPart(AbstractPart * part);

        String * mMessageID;
        AbstractPart * mPart;
        Data * mData;
    };

}

#endif

#endif
