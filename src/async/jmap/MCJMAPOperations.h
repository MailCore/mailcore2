#ifndef MAILCORE_MCJMAPOPERATIONS_H

#define MAILCORE_MCJMAPOPERATIONS_H

#include <MailCore/MCJMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class JMAPBlobUpload;
    class JMAPMessage;
    class JMAPSubmission;

    class MAILCORE_EXPORT JMAPFetchMailboxesOperation : public JMAPOperation {
    public:
        JMAPFetchMailboxesOperation();
        virtual ~JMAPFetchMailboxesOperation();
        virtual Array * mailboxes();
        virtual void main();
    private:
        Array * mMailboxes;
    };

    class MAILCORE_EXPORT JMAPQueryMessagesOperation : public JMAPOperation {
    public:
        JMAPQueryMessagesOperation();
        virtual ~JMAPQueryMessagesOperation();
        virtual void setText(String * text);
        virtual String * text();
        virtual void setMailboxID(String * mailboxID);
        virtual String * mailboxID();
        virtual void setPosition(unsigned int position);
        virtual unsigned int position();
        virtual void setLimit(unsigned int limit);
        virtual unsigned int limit();
        virtual Array * messageIDs();
        virtual void main();
    private:
        String * mText;
        String * mMailboxID;
        unsigned int mPosition;
        unsigned int mLimit;
        Array * mMessageIDs;
    };

    class MAILCORE_EXPORT JMAPFetchMessagesOperation : public JMAPOperation {
    public:
        JMAPFetchMessagesOperation();
        virtual ~JMAPFetchMessagesOperation();
        virtual void setMessageIDs(Array * messageIDs);
        virtual Array * messageIDs();
        virtual void setProperties(Array * properties);
        virtual Array * properties();
        virtual Array * messages();
        virtual void main();
    private:
        Array * mMessageIDs;
        Array * mProperties;
        Array * mMessages;
    };

    class MAILCORE_EXPORT JMAPUploadOperation : public JMAPOperation {
    public:
        JMAPUploadOperation();
        virtual ~JMAPUploadOperation();
        virtual void setData(Data * data);
        virtual Data * data();
        virtual void setContentType(String * contentType);
        virtual String * contentType();
        virtual void setAccountID(String * accountID);
        virtual String * accountID();
        virtual JMAPBlobUpload * upload();
        virtual void main();
    private:
        Data * mData;
        String * mContentType;
        String * mAccountID;
        JMAPBlobUpload * mUpload;
    };

    class MAILCORE_EXPORT JMAPDownloadOperation : public JMAPOperation {
    public:
        JMAPDownloadOperation();
        virtual ~JMAPDownloadOperation();
        virtual void setBlobID(String * blobID);
        virtual String * blobID();
        virtual void setName(String * name);
        virtual String * name();
        virtual void setAccept(String * accept);
        virtual String * accept();
        virtual void setAccountID(String * accountID);
        virtual String * accountID();
        virtual Data * data();
        virtual void main();
    private:
        String * mBlobID;
        String * mName;
        String * mAccept;
        String * mAccountID;
        Data * mData;
    };

    class MAILCORE_EXPORT JMAPCreateDraftOperation : public JMAPOperation {
    public:
        JMAPCreateDraftOperation();
        virtual ~JMAPCreateDraftOperation();
        virtual void setData(Data * data);
        virtual Data * data();
        virtual void setMailboxID(String * mailboxID);
        virtual String * mailboxID();
        virtual void setKeywords(Array * keywords);
        virtual Array * keywords();
        virtual JMAPMessage * message();
        virtual void main();
    private:
        Data * mData;
        String * mMailboxID;
        Array * mKeywords;
        JMAPMessage * mMessage;
    };

    class MAILCORE_EXPORT JMAPSendOperation : public JMAPOperation {
    public:
        JMAPSendOperation();
        virtual ~JMAPSendOperation();
        virtual void setData(Data * data);
        virtual Data * data();
        virtual void setMessageID(String * messageID);
        virtual String * messageID();
        virtual void setIdentityID(String * identityID);
        virtual String * identityID();
        virtual JMAPSubmission * submission();
        virtual void main();
    private:
        Data * mData;
        String * mMessageID;
        String * mIdentityID;
        JMAPSubmission * mSubmission;
    };

}

#endif

#endif
