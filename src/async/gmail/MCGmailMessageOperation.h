#ifndef MAILCORE_MCGMAILMESSAGEOPERATION_H

#define MAILCORE_MCGMAILMESSAGEOPERATION_H

#include <MailCore/MCGmailOperation.h>
#include <MailCore/MCGmailTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class GmailMessage;

    enum GmailMessageOperationKind {
        GmailMessageOperationKindDefault,
        GmailMessageOperationKindFormat,
        GmailMessageOperationKindMetadataHeaders,
    };

    class MAILCORE_EXPORT GmailMessageOperation : public GmailOperation {
    public:
        GmailMessageOperation();
        virtual ~GmailMessageOperation();

        virtual void setKind(GmailMessageOperationKind kind);
        virtual GmailMessageOperationKind kind();
        virtual void setMessageID(String * messageID);
        virtual String * messageID();
        virtual void setFormat(GmailMessageFormat format);
        virtual GmailMessageFormat format();
        virtual void setMetadataHeaders(Array * headers);
        virtual Array * metadataHeaders();
        virtual GmailMessage * message();
        virtual void main();

    private:
        GmailMessageOperationKind mKind;
        String * mMessageID;
        GmailMessageFormat mFormat;
        Array * mMetadataHeaders;
        GmailMessage * mMessage;
    };

}

#endif

#endif
