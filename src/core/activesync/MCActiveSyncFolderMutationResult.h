#ifndef MAILCORE_MCACTIVESYNCFOLDERMUTATIONRESULT_H

#define MAILCORE_MCACTIVESYNCFOLDERMUTATIONRESULT_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncFolderMutationResult : public Object {
    public:
        ActiveSyncFolderMutationResult();
        virtual ~ActiveSyncFolderMutationResult();

        virtual void setSyncKey(String * syncKey);
        virtual String * syncKey();
        virtual void setServerID(String * serverID);
        virtual String * serverID();
        virtual void setStatus(ActiveSyncFolderMutationStatus status);
        virtual ActiveSyncFolderMutationStatus status();

        virtual Object * copy();
        virtual String * description();

    private:
        String * mSyncKey;
        String * mServerID;
        ActiveSyncFolderMutationStatus mStatus;
        void init();
    };

}

#endif

#endif
