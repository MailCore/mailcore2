#include "MCActiveSyncSettingsResult.h"

#include "MCActiveSyncPrivate.h"

using namespace mailcore;

ActiveSyncSettingsResult::ActiveSyncSettingsResult()
{
    mStatus = ActiveSyncSettingsStatusUnknown;
    mDeviceInformationStatus = ActiveSyncSettingsDeviceInformationStatusUnknown;
}

ActiveSyncSettingsResult::~ActiveSyncSettingsResult()
{
}

void ActiveSyncSettingsResult::setStatus(ActiveSyncSettingsStatus value)
{
    mStatus = value;
}

ActiveSyncSettingsStatus ActiveSyncSettingsResult::status()
{
    return mStatus;
}

void ActiveSyncSettingsResult::setDeviceInformationStatus(ActiveSyncSettingsDeviceInformationStatus value)
{
    mDeviceInformationStatus = value;
}

ActiveSyncSettingsDeviceInformationStatus ActiveSyncSettingsResult::deviceInformationStatus()
{
    return mDeviceInformationStatus;
}

Object * ActiveSyncSettingsResult::copy()
{
    ActiveSyncSettingsResult * result = new ActiveSyncSettingsResult();
    result->setStatus(status());
    result->setDeviceInformationStatus(deviceInformationStatus());
    return result;
}
