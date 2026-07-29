#include "MCActiveSyncProvisionResult.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

void ActiveSyncProvisionResult::init()
{
    mStatus = ActiveSyncProvisionStatusUnknown;
    mPolicyStatus = ActiveSyncProvisionPolicyStatusUnknown;
    mPolicyKey = NULL;
}

ActiveSyncProvisionResult::ActiveSyncProvisionResult()
{
    init();
}

ActiveSyncProvisionResult::~ActiveSyncProvisionResult()
{
    MC_SAFE_RELEASE(mPolicyKey);
}

void ActiveSyncProvisionResult::setStatus(ActiveSyncProvisionStatus value)
{
    mStatus = value;
}

ActiveSyncProvisionStatus ActiveSyncProvisionResult::status()
{
    return mStatus;
}

void ActiveSyncProvisionResult::setPolicyStatus(ActiveSyncProvisionPolicyStatus value)
{
    mPolicyStatus = value;
}

ActiveSyncProvisionPolicyStatus ActiveSyncProvisionResult::policyStatus()
{
    return mPolicyStatus;
}

void ActiveSyncProvisionResult::setPolicyKey(String * value)
{
    MC_SET_STRING_FIELD(mPolicyKey, value);
}

String * ActiveSyncProvisionResult::policyKey()
{
    MC_GET_STRING_FIELD(mPolicyKey);
}

Object * ActiveSyncProvisionResult::copy()
{
    ActiveSyncProvisionResult * result = new ActiveSyncProvisionResult();
    result->setStatus(status());
    result->setPolicyStatus(policyStatus());
    result->setPolicyKey(policyKey());
    return result;
}
