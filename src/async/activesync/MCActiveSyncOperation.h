#ifndef MAILCORE_MCACTIVESYNCOPERATION_H

#define MAILCORE_MCACTIVESYNCOPERATION_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class ActiveSyncAsyncSession;

    class MAILCORE_EXPORT ActiveSyncOperation : public Operation {
    public:
        ActiveSyncOperation();
        virtual ~ActiveSyncOperation();

        virtual void setSession(ActiveSyncAsyncSession * session);
        virtual ActiveSyncAsyncSession * session();

        virtual void setError(ErrorCode error);
        virtual ErrorCode error();

        virtual void start();

    private:
        ActiveSyncAsyncSession * mSession;
        ErrorCode mError;
    };

}

#endif

#endif
