#ifndef MAILCORE_MCGMAILASYNCSESSION_H

#define MAILCORE_MCGMAILASYNCSESSION_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCGmailTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class GmailSession;
    class GmailOperation;
    class GmailProfileOperation;
    class GmailLabelsOperation;
    class GmailLabelOperation;
    class GmailMessagesOperation;
    class GmailMessageOperation;
    class GmailMessageDataOperation;
    class GmailAttachmentDataOperation;
    class GmailPartDataOperation;
    class GmailMessagePart;
    class AbstractPart;
    class GmailOperationQueueCallback;

    class MAILCORE_EXPORT GmailAsyncSession : public Object {
        friend class GmailOperation;

    public:
        GmailAsyncSession();
        virtual ~GmailAsyncSession();

        virtual void setUserID(String * userID);
        virtual String * userID();

        virtual void setOAuth2Token(String * token);
        virtual String * OAuth2Token();

        virtual void setUserAgent(String * userAgent);
        virtual String * userAgent();

        virtual void setTimeout(time_t timeout);
        virtual time_t timeout();

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

        virtual GmailProfileOperation * profileOperation();
        virtual GmailLabelsOperation * labelsOperation();
        virtual GmailLabelOperation * labelOperation(String * labelID);

        virtual GmailMessagesOperation * messagesOperation();
        virtual GmailMessagesOperation * messagesWithQueryOperation(String * query);
        virtual GmailMessagesOperation * messagesWithLabelOperation(String * labelID);

        virtual GmailMessageOperation * messageOperation(String * messageID);
        virtual GmailMessageOperation * messageWithFormatOperation(String * messageID,
                                                                   GmailMessageFormat format);
        virtual GmailMessageOperation * messageWithMetadataHeadersOperation(String * messageID,
                                                                            Array * headers);

        virtual GmailMessageDataOperation * messageDataOperation(String * messageID);
        virtual GmailAttachmentDataOperation * attachmentDataOperation(String * messageID,
                                                                       String * attachmentID);
        virtual GmailPartDataOperation * dataForPartOperation(String * messageID,
                                                              AbstractPart * part);

    private:
        virtual void runOperation(GmailOperation * operation);
        virtual GmailSession * session();

        GmailSession * mSession;
        OperationQueue * mQueue;
        GmailOperationQueueCallback * mQueueCallback;
        OperationQueueCallback * mOperationQueueCallback;
    };

}

#endif

#endif
