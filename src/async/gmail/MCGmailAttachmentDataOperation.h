#ifndef MAILCORE_MCGMAILATTACHMENTDATAOPERATION_H

#define MAILCORE_MCGMAILATTACHMENTDATAOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailAttachmentDataOperation : public GmailOperation {
    public:
        GmailAttachmentDataOperation();
        virtual ~GmailAttachmentDataOperation();

        virtual void setMessageID(String * messageID);
        virtual String * messageID();
        virtual void setAttachmentID(String * attachmentID);
        virtual String * attachmentID();
        virtual Data * data();
        virtual void main();

    private:
        String * mMessageID;
        String * mAttachmentID;
        Data * mData;
    };

}

#endif

#endif
