#ifndef MAILCORE_MCACTIVESYNCBODY_H

#define MAILCORE_MCACTIVESYNCBODY_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncBody : public Object {
    public:
        ActiveSyncBody();
        virtual ~ActiveSyncBody();

        virtual void setType(ActiveSyncBodyType type);
        virtual ActiveSyncBodyType type();
        virtual void setData(Data * data);
        virtual Data * data();
        virtual void setEstimatedDataSize(uint32_t estimatedDataSize);
        virtual uint32_t estimatedDataSize();
        virtual void setTruncated(bool truncated);
        virtual bool isTruncated();
        virtual void setNativeBodyType(ActiveSyncBodyType nativeBodyType);
        virtual ActiveSyncBodyType nativeBodyType();
        virtual void setMimeType(String * mimeType);
        virtual String * mimeType();
        virtual void setPreview(String * preview);
        virtual String * preview();
        virtual void setAttachments(Array * /* ActiveSyncAttachment */ attachments);
        virtual Array * /* ActiveSyncAttachment */ attachments();

        virtual Object * copy();
        virtual String * description();

    private:
        ActiveSyncBodyType mType;
        Data * mData;
        uint32_t mEstimatedDataSize;
        bool mTruncated;
        ActiveSyncBodyType mNativeBodyType;
        String * mMimeType;
        String * mPreview;
        Array * /* ActiveSyncAttachment */ mAttachments;
        void init();
    };

}

#endif

#endif
