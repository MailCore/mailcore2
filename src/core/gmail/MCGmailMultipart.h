#ifndef MAILCORE_MCGMAILMULTIPART_H

#define MAILCORE_MCGMAILMULTIPART_H

#include <MailCore/MCAbstractMultipart.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailMultipart : public AbstractMultipart {
    public:
        GmailMultipart();
        virtual ~GmailMultipart();

        virtual String * partID();
        virtual void setPartID(String * partID);

    private:
        String * mPartID;

        void init();
    };

}

#endif

#endif
