#ifndef MAILCORE_MCGMAILLABELOPERATION_H

#define MAILCORE_MCGMAILLABELOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class GmailLabel;

    class MAILCORE_EXPORT GmailLabelOperation : public GmailOperation {
        friend class GmailAsyncSession;

    public:
        GmailLabelOperation();
        virtual ~GmailLabelOperation();

        virtual String * labelID();
        virtual GmailLabel * label();
        virtual void main();

    private:
        virtual void setLabelID(String * labelID);

        String * mLabelID;
        GmailLabel * mLabel;
    };

}

#endif

#endif
