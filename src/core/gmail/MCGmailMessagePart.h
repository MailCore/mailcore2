#ifndef MAILCORE_MCGMAILMESSAGEPART_H

#define MAILCORE_MCGMAILMESSAGEPART_H

#include <MailCore/MCAbstractMessagePart.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailMessagePart : public AbstractMessagePart {
    public:
        GmailMessagePart();
        virtual ~GmailMessagePart();

        virtual String * partID();
        virtual void setPartID(String * partID);

        virtual Array * /* GmailMessageHeader */ headers();
        virtual void setHeaders(Array * headers);

        virtual String * attachmentID();
        virtual void setAttachmentID(String * attachmentID);

        virtual uint32_t size();
        virtual void setSize(uint32_t size);

        virtual Data * data();
        virtual void setData(Data * data);

        virtual Array * /* GmailMessagePart */ parts();
        virtual void setParts(Array * parts);

    private:
        String * mPartID;
        Array * mHeaders;
        String * mAttachmentID;
        uint32_t mSize;
        Data * mData;
        Array * mParts;

        void init();
    };

}

#endif

#endif
