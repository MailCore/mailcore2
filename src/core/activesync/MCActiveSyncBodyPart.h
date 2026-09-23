#ifndef MAILCORE_MCACTIVESYNCBODYPART_H

#define MAILCORE_MCACTIVESYNCBODYPART_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncBodyPart : public Object {
    public:
        ActiveSyncBodyPart();
        virtual ~ActiveSyncBodyPart();

        virtual void setStatus(ActiveSyncItemOperationsStatus status);
        virtual ActiveSyncItemOperationsStatus status();
        virtual void setType(ActiveSyncBodyType type);
        virtual ActiveSyncBodyType type();
        virtual void setData(Data * data);
        virtual Data * data();
        virtual void setEstimatedDataSize(uint32_t estimatedDataSize);
        virtual uint32_t estimatedDataSize();
        virtual void setTruncated(bool truncated);
        virtual bool isTruncated();
        virtual void setPreview(String * preview);
        virtual String * preview();

        virtual Object * copy();
        virtual String * description();

    private:
        ActiveSyncItemOperationsStatus mStatus;
        ActiveSyncBodyType mType;
        Data * mData;
        uint32_t mEstimatedDataSize;
        bool mTruncated;
        String * mPreview;
        void init();
    };

}

#endif

#endif
