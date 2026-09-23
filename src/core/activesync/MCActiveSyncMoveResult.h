#ifndef MAILCORE_MCACTIVESYNCMOVERESULT_H

#define MAILCORE_MCACTIVESYNCMOVERESULT_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncMoveResult : public Object {
    public:
        ActiveSyncMoveResult();
        virtual ~ActiveSyncMoveResult();

        virtual void setStatus(ActiveSyncMoveStatus status);
        virtual ActiveSyncMoveStatus status();
        virtual void setResponses(Array * responses);
        virtual Array * /* ActiveSyncMoveResponse */ responses();

        virtual Object * copy();
        virtual String * description();

    private:
        ActiveSyncMoveStatus mStatus;
        Array * /* ActiveSyncMoveResponse */ mResponses;
        void init();
    };

}

#endif

#endif
