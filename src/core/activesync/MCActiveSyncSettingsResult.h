#ifndef MAILCORE_MCACTIVESYNCSETTINGSRESULT_H

#define MAILCORE_MCACTIVESYNCSETTINGSRESULT_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncSettingsResult : public Object {
    public:
        ActiveSyncSettingsResult();
        virtual ~ActiveSyncSettingsResult();
        virtual void setStatus(ActiveSyncSettingsStatus status);
        virtual ActiveSyncSettingsStatus status();
        virtual void setDeviceInformationStatus(ActiveSyncSettingsDeviceInformationStatus status);
        virtual ActiveSyncSettingsDeviceInformationStatus deviceInformationStatus();
        virtual Object * copy();
    private:
        ActiveSyncSettingsStatus mStatus;
        ActiveSyncSettingsDeviceInformationStatus mDeviceInformationStatus;
    };

}

#endif

#endif
