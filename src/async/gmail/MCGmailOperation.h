#ifndef MAILCORE_MCGMAILOPERATION_H

#define MAILCORE_MCGMAILOPERATION_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class GmailAsyncSession;
    class GmailSession;

    class MAILCORE_EXPORT GmailOperation : public Operation {
        friend class GmailAsyncSession;

    public:
        GmailOperation();
        virtual ~GmailOperation();

        virtual ErrorCode error();

        virtual void start();

    protected:
        virtual GmailAsyncSession * session();
        virtual GmailSession * syncSession();
        virtual void setError(ErrorCode error);

    private:
        virtual void setSession(GmailAsyncSession * session);

        GmailAsyncSession * mSession;
        ErrorCode mError;
    };

}

#endif

#endif
