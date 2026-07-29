#include "MCActiveSyncItemEstimateResult.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

ActiveSyncItemEstimateResult::ActiveSyncItemEstimateResult()
{
    mStatus = ActiveSyncItemEstimateStatusUnknown;
    mCollectionStatus = ActiveSyncItemEstimateStatusUnknown;
    mEstimate = 0;
    mEmptyResponse = false;
}

ActiveSyncItemEstimateResult::~ActiveSyncItemEstimateResult()
{
}

void ActiveSyncItemEstimateResult::setStatus(ActiveSyncItemEstimateStatus value)
{
    mStatus = value;
}

ActiveSyncItemEstimateStatus ActiveSyncItemEstimateResult::status()
{
    return mStatus;
}

void ActiveSyncItemEstimateResult::setCollectionStatus(ActiveSyncItemEstimateStatus value)
{
    mCollectionStatus = value;
}

ActiveSyncItemEstimateStatus ActiveSyncItemEstimateResult::collectionStatus()
{
    return mCollectionStatus;
}

void ActiveSyncItemEstimateResult::setEstimate(uint32_t value)
{
    mEstimate = value;
}

uint32_t ActiveSyncItemEstimateResult::estimate()
{
    return mEstimate;
}

void ActiveSyncItemEstimateResult::setEmptyResponse(bool value)
{
    mEmptyResponse = value;
}

bool ActiveSyncItemEstimateResult::emptyResponse()
{
    return mEmptyResponse;
}

Object * ActiveSyncItemEstimateResult::copy()
{
    ActiveSyncItemEstimateResult * result = new ActiveSyncItemEstimateResult();
    result->setStatus(status());
    result->setCollectionStatus(collectionStatus());
    result->setEstimate(estimate());
    result->setEmptyResponse(emptyResponse());
    return result;
}
