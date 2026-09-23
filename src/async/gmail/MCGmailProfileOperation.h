#ifndef MAILCORE_MCGMAILPROFILEOPERATION_H

#define MAILCORE_MCGMAILPROFILEOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class GmailProfile;

    class MAILCORE_EXPORT GmailProfileOperation : public GmailOperation {
    public:
        GmailProfileOperation();
        virtual ~GmailProfileOperation();

        virtual GmailProfile * profile();
        virtual void main();

    private:
        GmailProfile * mProfile;
    };

}

#endif

#endif
