#ifndef MAILCORE_MCACTIVESYNCITEMESTIMATERESULT_H

#define MAILCORE_MCACTIVESYNCITEMESTIMATERESULT_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncItemEstimateResult : public Object {
    public:
        ActiveSyncItemEstimateResult();
        virtual ~ActiveSyncItemEstimateResult();
        virtual void setStatus(ActiveSyncItemEstimateStatus status);
        virtual ActiveSyncItemEstimateStatus status();
        virtual void setCollectionStatus(ActiveSyncItemEstimateStatus status);
        virtual ActiveSyncItemEstimateStatus collectionStatus();
        virtual void setEstimate(uint32_t estimate);
        virtual uint32_t estimate();
        virtual void setEmptyResponse(bool emptyResponse);
        virtual bool emptyResponse();
        virtual Object * copy();
    private:
        ActiveSyncItemEstimateStatus mStatus;
        ActiveSyncItemEstimateStatus mCollectionStatus;
        uint32_t mEstimate;
        bool mEmptyResponse;
    };

}

#endif

#endif
