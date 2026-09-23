#ifndef MAILCORE_MCGMAILLABELSOPERATION_H

#define MAILCORE_MCGMAILLABELSOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailLabelsOperation : public GmailOperation {
    public:
        GmailLabelsOperation();
        virtual ~GmailLabelsOperation();

        virtual Array * /* GmailLabel */ labels();
        virtual void main();

    private:
        Array * mLabels;
    };

}

#endif

#endif
