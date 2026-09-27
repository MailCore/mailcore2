#ifndef MAILCORE_MCGMAILMESSAGEOPERATION_H

#define MAILCORE_MCGMAILMESSAGEOPERATION_H

#include <MailCore/MCGmailOperation.h>
#include <MailCore/MCGmailTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class GmailMessage;

    class MAILCORE_EXPORT GmailMessageOperation : public GmailOperation {
        friend class GmailAsyncSession;

    public:
        GmailMessageOperation();
        virtual ~GmailMessageOperation();

        virtual String * messageID();
        virtual GmailMessageFormat format();
        virtual Array * metadataHeaders();
        virtual GmailMessage * message();
        virtual void main();

    private:
        enum GmailMessageOperationKind {
            GmailMessageOperationKindDefault,
            GmailMessageOperationKindFormat,
            GmailMessageOperationKindMetadataHeaders,
        };

        virtual void setKind(GmailMessageOperationKind kind);
        virtual GmailMessageOperationKind kind();
        virtual void setMessageID(String * messageID);
        virtual void setFormat(GmailMessageFormat format);
        virtual void setMetadataHeaders(Array * headers);

        GmailMessageOperationKind mKind;
        String * mMessageID;
        GmailMessageFormat mFormat;
        Array * mMetadataHeaders;
        GmailMessage * mMessage;
    };

}

#endif

#endif
