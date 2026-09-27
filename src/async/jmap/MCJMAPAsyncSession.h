#ifndef MAILCORE_MCJMAPASYNCSESSION_H

#define MAILCORE_MCJMAPASYNCSESSION_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class JMAPSession;
    class JMAPOperation;
    class JMAPFetchMailboxesOperation;
    class JMAPQueryMessagesOperation;
    class JMAPFetchMessagesOperation;
    class JMAPUploadOperation;
    class JMAPDownloadOperation;
    class JMAPCreateDraftOperation;
    class JMAPSendOperation;
    class JMAPOperationQueueCallback;

    class MAILCORE_EXPORT JMAPAsyncSession : public Object {
    public:
        JMAPAsyncSession();
        virtual ~JMAPAsyncSession();

        virtual void setSessionURL(String * sessionURL);
        virtual String * sessionURL();
        virtual void setDomainOrEmail(String * domainOrEmail);
        virtual String * domainOrEmail();
        virtual void setAccountID(String * accountID);
        virtual String * accountID();
        virtual void setUsername(String * username);
        virtual String * username();
        virtual void setOAuth2Token(String * token);
        virtual String * OAuth2Token();
        virtual void setTimeout(time_t timeout);
        virtual time_t timeout();
        virtual void setCheckCertificateEnabled(bool enabled);
        virtual bool isCheckCertificateEnabled();

        virtual int lastHTTPStatus();
        virtual String * lastErrorMessage();

#ifdef __APPLE__
        virtual void setDispatchQueue(dispatch_queue_t dispatchQueue);
        virtual dispatch_queue_t dispatchQueue();
#endif

        virtual void setOperationQueueCallback(OperationQueueCallback * callback);
        virtual OperationQueueCallback * operationQueueCallback();
        virtual bool isOperationQueueRunning();
        virtual void cancelAllOperations();

        virtual JMAPOperation * connectOperation();
        virtual JMAPOperation * discoverOperation();
        virtual JMAPFetchMailboxesOperation * fetchMailboxesOperation();
        virtual JMAPQueryMessagesOperation * queryMessagesWithTextOperation(String * text, unsigned int position, unsigned int limit);
        virtual JMAPQueryMessagesOperation * queryMessagesInMailboxOperation(String * mailboxID, unsigned int position, unsigned int limit);
        virtual JMAPFetchMessagesOperation * fetchMessagesOperation(Array * messageIDs, Array * properties);
        virtual JMAPUploadOperation * uploadOperation(Data * data, String * contentType, String * accountID);
        virtual JMAPDownloadOperation * downloadOperation(String * blobID, String * name, String * accept, String * accountID);
        virtual JMAPCreateDraftOperation * createDraftOperation(Data * data, String * mailboxID, Array * keywords);
        virtual JMAPOperation * updateDraftOperation(String * messageID, Array * mailboxIDs, Array * keywords);
        virtual JMAPOperation * deleteDraftOperation(String * messageID);
        virtual JMAPSendOperation * sendMessageOperation(String * messageID, String * identityID);
        virtual JMAPSendOperation * sendDataOperation(Data * data, String * identityID);

    public: // private
        virtual void runOperation(JMAPOperation * operation);
        virtual JMAPSession * session();

    private:
        JMAPSession * mSession;
        OperationQueue * mQueue;
        JMAPOperationQueueCallback * mQueueCallback;
        OperationQueueCallback * mOperationQueueCallback;
    };

}

#endif

#endif
