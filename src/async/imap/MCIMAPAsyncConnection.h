#ifndef __MAILCORE_MCIMAPASYNCCONNECTION_H

#define __MAILCORE_MCIMAPASYNCCONNECTION_H

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCMessageConstants.h>

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

	class IMAPAsyncConnection : public Object {
	private:
		IMAPSession * mSession;
		OperationQueue * mQueue;
        char mDelimiter;
        IMAPNamespace * mDefaultNamespace;
        String * mLastFolder;
		
		void queue(IMAPOperation * op);
		
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
		
		virtual IMAPCopyMessagesOperation * copyMessagesOperation(String * folder, Array * uids, String * destFolder);
		
		virtual IMAPOperation * expungeOperation(String * folder);
		
		virtual IMAPFetchMessagesOperation * fetchMessagesByUIDOperation(String * folder, IMAPMessagesRequestKind requestKind,
			uint32_t firstUID, uint32_t lastUID);
		virtual IMAPFetchMessagesOperation * fetchMessagesByNumberOperation(String * folder, IMAPMessagesRequestKind requestKind,
			uint32_t firstNumber, uint32_t lastNumber);
		virtual IMAPFetchMessagesOperation * fetchMessagesByUIDOperation(String * folder, IMAPMessagesRequestKind requestKind,
			Array * uids);
		virtual IMAPFetchMessagesOperation * fetchMessagesByNumberOperation(String * folder, IMAPMessagesRequestKind requestKind,
			Array * numbers);
		virtual IMAPFetchContentOperation * fetchMessageByUIDOperation(String * folder, uint32_t uid);
		virtual IMAPFetchContentOperation * fetchMessageAttachmentByUIDOperation(String * folder, uint32_t uid, String * partID,
		    Encoding encoding);
		
		virtual IMAPOperation * storeFlagsOperation(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, MessageFlag flags);
		virtual IMAPOperation * storeLabelsOperation(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, Array * labels);
		
		virtual IMAPSearchOperation * searchOperation(String * folder, IMAPSearchKind kind, String * searchString);
		virtual IMAPSearchOperation * searchOperation(String * folder, IMAPSearchExpression * expression);
		
		virtual IMAPIdleOperation * idleOperation(String * folder, uint32_t lastKnownUID);
		
		virtual IMAPFetchNamespaceOperation * fetchNamespaceOperation();
		
		virtual IMAPIdentityOperation * identityOperation(String * vendor, String * name, String * version);
        
        virtual IMAPOperation * checkAccountOperation();
        
        // private
        virtual void runOperation(IMAPOperation * operation);
        virtual IMAPSession * session();
        
        virtual unsigned int operationsCount();
        
		virtual void setLastFolder(String * folder);
		virtual String * lastFolder();
	};
}

#endif
