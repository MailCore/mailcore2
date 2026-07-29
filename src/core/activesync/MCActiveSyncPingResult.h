#ifndef MAILCORE_MCACTIVESYNCPINGRESULT_H

#define MAILCORE_MCACTIVESYNCPINGRESULT_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncPingResult : public Object {
    public:
        ActiveSyncPingResult();
        virtual ~ActiveSyncPingResult();
        virtual void setStatus(ActiveSyncPingStatus status);
        virtual ActiveSyncPingStatus status();
        virtual void setChangedCollectionIDs(Array * /* String */ changedCollectionIDs);
        virtual Array * /* String */ changedCollectionIDs();
        virtual Object * copy();
    private:
        ActiveSyncPingStatus mStatus;
        Array * /* String */ mChangedCollectionIDs;
    };

}

#endif

#endif
