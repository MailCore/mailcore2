#ifndef MAILCORE_MCACTIVESYNCFOLDERSYNCRESULT_H

#define MAILCORE_MCACTIVESYNCFOLDERSYNCRESULT_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncFolderSyncResult : public Object {
    public:
        ActiveSyncFolderSyncResult();
        virtual ~ActiveSyncFolderSyncResult();

        virtual void setSyncKey(String * syncKey);
        virtual String * syncKey();
        virtual void setStatus(ActiveSyncFolderSyncStatus status);
        virtual ActiveSyncFolderSyncStatus status();
        virtual void setAdded(Array * /* ActiveSyncFolder */ added);
        virtual Array * /* ActiveSyncFolder */ added();
        virtual void setUpdated(Array * /* ActiveSyncFolder */ updated);
        virtual Array * /* ActiveSyncFolder */ updated();
        virtual void setDeleted(Array * /* String */ deleted);
        virtual Array * /* String */ deleted();

        virtual Object * copy();
        virtual String * description();

    private:
        String * mSyncKey;
        ActiveSyncFolderSyncStatus mStatus;
        Array * /* ActiveSyncFolder */ mAdded;
        Array * /* ActiveSyncFolder */ mUpdated;
        Array * /* String */ mDeleted;
        void init();
    };

}

#endif

#endif
