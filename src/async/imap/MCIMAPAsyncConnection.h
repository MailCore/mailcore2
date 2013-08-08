#ifndef __MAILCORE_MCIMAPASYNCCONNECTION_H

#define __MAILCORE_MCIMAPASYNCCONNECTION_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPOperation;
    class IMAPFetchFoldersOperation;
    class IMAPAppendMessageOperation;
    class IMAPCopyMessagesOperation;
    class IMAPFetchMessagesOperation;
    class IMAPFetchContentOperation;
    class IMAPIdleOperation;
    class IMAPFolderInfoOperation;
    class IMAPFolderStatusOperation;
    class IMAPSession;
    class IMAPNamespace;
    class IMAPSearchOperation;
    class IMAPSearchExpression;
    class IMAPFetchNamespaceOperation;
    class IMAPIdentityOperation;
    class IMAPCapabilityOperation;
    class IMAPGetQuotaOperation;
    class IMAPOperationQueueCallback;
    class IMAPAsyncSession;
    class IMAPConnectionLogger;
    class IMAPMessageRenderingOperation;
    class IMAPMessage;
    
    class IMAPAsyncConnection : public Object {
    public:
        IMAPAsyncConnection();
        virtual ~IMAPAsyncConnection();
        
        virtual void setHostname(String * hostname);
        virtual String * hostname();
        
        virtual void setPort(unsigned int port);
        virtual unsigned int port();
        
        virtual void setUsername(String * username);
        virtual String * username();
        
        virtual void setPassword(String * password);
        virtual String * password();
        
        virtual void setOAuth2Token(String * token);
        virtual String * OAuth2Token();
        
        virtual void setAuthType(AuthType authType);
        virtual AuthType authType();
        
        virtual void setConnectionType(ConnectionType connectionType);
        virtual ConnectionType connectionType();
        
        virtual void setTimeout(time_t timeout);
        virtual time_t timeout();
        
        virtual void setCheckCertificateEnabled(bool enabled);
        virtual bool isCheckCertificateEnabled();
        
        virtual void setVoIPEnabled(bool enabled);
        virtual bool isVoIPEnabled();
        
        virtual void setDelimiter(char delimiter);
        virtual char delimiter();
        
        virtual void setDefaultNamespace(IMAPNamespace * ns);
        virtual IMAPNamespace * defaultNamespace();
        
        virtual void setConnectionLogger(ConnectionLogger * logger);
        virtual ConnectionLogger * connectionLogger();
        
        virtual IMAPFolderInfoOperation * folderInfoOperation(String * folder);
        virtual IMAPFolderStatusOperation * folderStatusOperation(String * folder);
        
        virtual IMAPFetchFoldersOperation * fetchSubscribedFoldersOperation();
        virtual IMAPFetchFoldersOperation * fetchAllFoldersOperation();
        
        virtual IMAPOperation * renameFolderOperation(String * folder, String * otherName);
        virtual IMAPOperation * deleteFolderOperation(String * folder);
        virtual IMAPOperation * createFolderOperation(String * folder);
        
        virtual IMAPOperation * subscribeFolderOperation(String * folder);
        virtual IMAPOperation * unsubscribeFolderOperation(String * folder);
        
        virtual IMAPAppendMessageOperation * appendMessageOperation(String * folder, Data * messageData, MessageFlag flags);
        
        virtual IMAPCopyMessagesOperation * copyMessagesOperation(String * folder, IndexSet * uids, String * destFolder);
        
        virtual IMAPOperation * expungeOperation(String * folder);
        
        virtual IMAPFetchMessagesOperation * fetchMessagesByUIDOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                         IndexSet * uids);
        virtual IMAPFetchMessagesOperation * fetchMessagesByNumberOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                            IndexSet * numbers);
        virtual IMAPFetchMessagesOperation * syncMessagesByUID(String * folder, IMAPMessagesRequestKind requestKind,
                                                               IndexSet * uids, uint64_t modSeq);
        
        virtual IMAPFetchContentOperation * fetchMessageByUIDOperation(String * folder, uint32_t uid);
        virtual IMAPFetchContentOperation * fetchMessageAttachmentByUIDOperation(String * folder, uint32_t uid, String * partID,
                                                                                 Encoding encoding);
        
        virtual IMAPOperation * storeFlagsOperation(String * folder, IndexSet * uids, IMAPStoreFlagsRequestKind kind, MessageFlag flags);
        virtual IMAPOperation * storeLabelsOperation(String * folder, IndexSet * uids, IMAPStoreFlagsRequestKind kind, Array * labels);
        
        virtual IMAPSearchOperation * searchOperation(String * folder, IMAPSearchKind kind, String * searchString);
        virtual IMAPSearchOperation * searchOperation(String * folder, IMAPSearchExpression * expression);
        
        virtual IMAPIdleOperation * idleOperation(String * folder, uint32_t lastKnownUID);
        
        virtual IMAPFetchNamespaceOperation * fetchNamespaceOperation();
        
        virtual IMAPIdentityOperation * identityOperation(String * vendor, String * name, String * version);
        
        virtual IMAPOperation * checkAccountOperation();
        
        virtual IMAPCapabilityOperation * capabilityOperation();
        virtual IMAPGetQuotaOperation * getQuotaOperation();
        
        virtual IMAPMessageRenderingOperation * htmlRenderingOperation(IMAPMessage * message, String * folder);
        virtual IMAPMessageRenderingOperation * htmlBodyRenderingOperation(IMAPMessage * message, String * folder);
        virtual IMAPMessageRenderingOperation * plainTextRenderingOperation(IMAPMessage * message, String * folder);
        virtual IMAPMessageRenderingOperation * plainTextBodyRenderingOperation(IMAPMessage * message, String * folder);
        
        
    private:
        IMAPSession * mSession;
        OperationQueue * mQueue;
        char mDelimiter;
        IMAPNamespace * mDefaultNamespace;
        String * mLastFolder;
        IMAPOperationQueueCallback * mQueueCallback;
        IMAPAsyncSession * mOwner;
        ConnectionLogger * mConnectionLogger;
        IMAPConnectionLogger * mInternalLogger;
        pthread_mutex_t mConnectionLoggerLock;
        
        virtual void tryAutomaticDisconnectAfterDelay(void * context);
        virtual IMAPMessageRenderingOperation * renderingOperation(IMAPMessage * message,
                                                                   String * folder,
                                                                   IMAPMessageRenderingType type);
        
    public: // private
        virtual void runOperation(IMAPOperation * operation);
        virtual IMAPSession * session();
        
        virtual unsigned int operationsCount();
        
        virtual void setLastFolder(String * folder);
        virtual String * lastFolder();
        
        virtual void tryAutomaticDisconnect();
        virtual void queueStartRunning();
        virtual void queueStoppedRunning();
        
        virtual void setOwner(IMAPAsyncSession * owner);
        virtual IMAPAsyncSession * owner();

        virtual void logConnection(ConnectionLogType logType, Data * buffer);
    };
}

#endif

#endif
