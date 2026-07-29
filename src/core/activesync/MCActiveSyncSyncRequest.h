#ifndef MAILCORE_MCACTIVESYNCSYNCREQUEST_H

#define MAILCORE_MCACTIVESYNCSYNCREQUEST_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncSyncRequest : public Object {
    public:
        ActiveSyncSyncRequest();
        virtual ~ActiveSyncSyncRequest();

        virtual void setCollectionID(String * collectionID);
        virtual String * collectionID();
        virtual void setSyncKey(String * syncKey);
        virtual String * syncKey();
        virtual void setCollectionClass(String * collectionClass);
        virtual String * collectionClass();
        virtual void setGetChanges(bool getChanges);
        virtual bool getChanges();
        virtual void setDeletesAsMoves(bool deletesAsMoves);
        virtual bool deletesAsMoves();
        virtual void setFilterType(ActiveSyncFilterType filterType);
        virtual ActiveSyncFilterType filterType();
        virtual bool hasFilterType();
        virtual void setConflict(uint32_t conflict);
        virtual uint32_t conflict();
        virtual bool hasConflict();
        virtual void setWindowSize(uint32_t windowSize);
        virtual uint32_t windowSize();
        virtual void setBodyPreference(ActiveSyncBodyType bodyType, uint32_t truncationSize);
        virtual ActiveSyncBodyType bodyPreferenceType();
        virtual uint32_t bodyPreferenceTruncationSize();
        virtual bool hasBodyPreference();

        virtual Object * copy();
        virtual String * description();

    private:
        String * mCollectionID;
        String * mSyncKey;
        String * mCollectionClass;
        bool mGetChanges;
        bool mDeletesAsMoves;
        bool mHasFilterType;
        ActiveSyncFilterType mFilterType;
        bool mHasConflict;
        uint32_t mConflict;
        uint32_t mWindowSize;
        bool mHasBodyPreference;
        ActiveSyncBodyType mBodyPreferenceType;
        uint32_t mBodyPreferenceTruncationSize;
        void init();
    };

}

#endif

#endif
