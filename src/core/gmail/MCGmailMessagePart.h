#ifndef MAILCORE_MCGMAILMESSAGEPART_H

#define MAILCORE_MCGMAILMESSAGEPART_H

#include <MailCore/MCAbstractMessagePart.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailMessagePart : public AbstractMessagePart {
    public:
        GmailMessagePart();
        virtual ~GmailMessagePart();

        virtual String * partID();
        virtual void setPartID(String * partID);

    private:
        String * mPartID;

        void init();
    };

}

#endif

#endif
