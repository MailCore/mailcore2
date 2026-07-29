#ifndef MAILCORE_MCACTIVESYNCPROVISIONRESULT_H

#define MAILCORE_MCACTIVESYNCPROVISIONRESULT_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncProvisionResult : public Object {
    public:
        ActiveSyncProvisionResult();
        virtual ~ActiveSyncProvisionResult();
        virtual void setStatus(ActiveSyncProvisionStatus status);
        virtual ActiveSyncProvisionStatus status();
        virtual void setPolicyStatus(ActiveSyncProvisionPolicyStatus policyStatus);
        virtual ActiveSyncProvisionPolicyStatus policyStatus();
        virtual void setPolicyKey(String * policyKey);
        virtual String * policyKey();
        virtual Object * copy();
    private:
        ActiveSyncProvisionStatus mStatus;
        ActiveSyncProvisionPolicyStatus mPolicyStatus;
        String * mPolicyKey;
        void init();
    };

}

#endif

#endif
