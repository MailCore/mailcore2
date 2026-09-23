#ifndef MAILCORE_MCGMAILOPERATION_H

#define MAILCORE_MCGMAILOPERATION_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class GmailAsyncSession;

    class MAILCORE_EXPORT GmailOperation : public Operation {
    public:
        GmailOperation();
        virtual ~GmailOperation();

        virtual void setSession(GmailAsyncSession * session);
        virtual GmailAsyncSession * session();

        virtual void setError(ErrorCode error);
        virtual ErrorCode error();

        virtual void start();

    private:
        GmailAsyncSession * mSession;
        ErrorCode mError;
    };

}

#endif

#endif
