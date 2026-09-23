#ifndef MAILCORE_MCGMAILPART_H

#define MAILCORE_MCGMAILPART_H

#include <MailCore/MCAbstractPart.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailPart : public AbstractPart {
    public:
        GmailPart();
        virtual ~GmailPart();

        virtual String * partID();
        virtual void setPartID(String * partID);

        virtual String * attachmentID();
        virtual void setAttachmentID(String * attachmentID);

        virtual uint32_t size();
        virtual void setSize(uint32_t size);

        virtual Data * data();
        virtual void setData(Data * data);

    private:
        String * mPartID;
        String * mAttachmentID;
        uint32_t mSize;
        Data * mData;

        void init();
    };

}

#endif

#endif
