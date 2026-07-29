#ifndef MAILCORE_MCACTIVESYNCATTACHMENT_H

#define MAILCORE_MCACTIVESYNCATTACHMENT_H

#include <MailCore/MCAbstractPart.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncAttachment : public AbstractPart {
    public:
        ActiveSyncAttachment();
        virtual ~ActiveSyncAttachment();

        virtual void setMethod(ActiveSyncAttachmentMethod method);
        virtual ActiveSyncAttachmentMethod method();
        virtual void setEstimatedDataSize(uint32_t estimatedDataSize);
        virtual uint32_t estimatedDataSize();

        virtual Object * copy();
        virtual String * description();

    private:
        ActiveSyncAttachmentMethod mMethod;
        uint32_t mEstimatedDataSize;
        void init();
    };

}

#endif

#endif
