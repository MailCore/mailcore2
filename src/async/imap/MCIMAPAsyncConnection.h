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
	class IMAPSession;
	class IMAPNamespace;
    class IMAPSearchOperation;
    class IMAPSearchExpression;
    class IMAPFetchNamespaceOperation;
    class IMAPIdentityOperation;
    class IMAPCapabilityOperation;

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

		virtual IMAPFolderInfoOperation * folderInfoOperation(String * folder);
		
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
        
    private:
		IMAPSession * mSession;
		OperationQueue * mQueue;
        char mDelimiter;
        IMAPNamespace * mDefaultNamespace;
        String * mLastFolder;
		
		void queue(IMAPOperation * op);
		
    public: // private
        virtual void runOperation(IMAPOperation * operation);
        virtual IMAPSession * session();
        
        virtual unsigned int operationsCount();
        
		virtual void setLastFolder(String * folder);
		virtual String * lastFolder();
    };
}

#endif

#endif
