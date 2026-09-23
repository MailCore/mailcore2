#ifndef MAILCORE_MCGMAILLABELOPERATION_H

#define MAILCORE_MCGMAILLABELOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class GmailLabel;

    class MAILCORE_EXPORT GmailLabelOperation : public GmailOperation {
    public:
        GmailLabelOperation();
        virtual ~GmailLabelOperation();

        virtual void setLabelID(String * labelID);
        virtual String * labelID();
        virtual GmailLabel * label();
        virtual void main();

    private:
        String * mLabelID;
        GmailLabel * mLabel;
    };

}

#endif

#endif
