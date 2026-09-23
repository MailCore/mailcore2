#ifndef MAILCORE_MCACTIVESYNCATTACHMENTDATA_H

#define MAILCORE_MCACTIVESYNCATTACHMENTDATA_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncAttachmentData : public Object {
    public:
        ActiveSyncAttachmentData();
        virtual ~ActiveSyncAttachmentData();

        virtual void setStatus(ActiveSyncItemOperationsStatus status);
        virtual ActiveSyncItemOperationsStatus status();
        virtual void setFileReference(String * fileReference);
        virtual String * fileReference();
        virtual void setRange(String * range);
        virtual String * range();
        virtual void setTotal(uint32_t total);
        virtual uint32_t total();
        virtual void setData(Data * data);
        virtual Data * data();

        virtual Object * copy();
        virtual String * description();

    private:
        ActiveSyncItemOperationsStatus mStatus;
        String * mFileReference;
        String * mRange;
        uint32_t mTotal;
        Data * mData;
        void init();
    };

}

#endif

#endif
