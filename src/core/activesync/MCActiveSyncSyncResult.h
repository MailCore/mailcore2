#ifndef MAILCORE_MCACTIVESYNCSYNCRESULT_H

#define MAILCORE_MCACTIVESYNCSYNCRESULT_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncSyncResult : public Object {
    public:
        ActiveSyncSyncResult();
        virtual ~ActiveSyncSyncResult();

        virtual void setSyncKey(String * syncKey);
        virtual String * syncKey();
        virtual void setStatus(ActiveSyncSyncStatus status);
        virtual ActiveSyncSyncStatus status();
        virtual void setMoreAvailable(bool moreAvailable);
        virtual bool moreAvailable();
        virtual void setEmptyResponse(bool emptyResponse);
        virtual bool emptyResponse();
        virtual void setSyncKeyFromResponse(bool syncKeyFromResponse);
        virtual bool syncKeyFromResponse();
        virtual void setAdded(Array * /* ActiveSyncMessage */ added);
        virtual Array * /* ActiveSyncMessage */ added();
        virtual void setChanged(Array * /* ActiveSyncMessage */ changed);
        virtual Array * /* ActiveSyncMessage */ changed();
        virtual void setDeleted(Array * /* String */ deleted);
        virtual Array * /* String */ deleted();

        virtual Object * copy();
        virtual String * description();

    private:
        String * mSyncKey;
        ActiveSyncSyncStatus mStatus;
        bool mMoreAvailable;
        bool mEmptyResponse;
        bool mSyncKeyFromResponse;
        Array * /* ActiveSyncMessage */ mAdded;
        Array * /* ActiveSyncMessage */ mChanged;
        Array * /* String */ mDeleted;
        void init();
    };

}

#endif

#endif
