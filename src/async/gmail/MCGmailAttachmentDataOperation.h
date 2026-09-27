#ifndef MAILCORE_MCGMAILATTACHMENTDATAOPERATION_H

#define MAILCORE_MCGMAILATTACHMENTDATAOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailAttachmentDataOperation : public GmailOperation {
        friend class GmailAsyncSession;

    public:
        GmailAttachmentDataOperation();
        virtual ~GmailAttachmentDataOperation();

        virtual String * messageID();
        virtual String * attachmentID();
        virtual Data * data();
        virtual void main();

    private:
        virtual void setMessageID(String * messageID);
        virtual void setAttachmentID(String * attachmentID);

        String * mMessageID;
        String * mAttachmentID;
        Data * mData;
    };

}

#endif

#endif
