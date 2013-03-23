//
//  MCIMAPAccount.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/17/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPAccount__
#define __mailcore2__MCIMAPAccount__

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCMessageConstants.h>

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
	class IMAPNamespace;
    class IMAPSearchOperation;
    class IMAPSearchExpression;
    class IMAPFetchNamespaceOperation;
    class IMAPIdentityOperation;
    class IMAPAsyncConnection;
    class IMAPCapabilityOperation;
    
	class IMAPAsyncSession : public Object {
	public:
		IMAPAsyncSession();
		virtual ~IMAPAsyncSession();
		
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
        
        virtual void setAllowsFolderConcurrentAccessEnabled(bool enabled);
        virtual bool allowsFolderConcurrentAccessEnabled();
        
        virtual void setMaximumConnections(unsigned int maxConnections);
        virtual unsigned int maximumConnections();
        
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
                                                                         IndexSet * indexes);
		virtual IMAPFetchMessagesOperation * fetchMessagesByNumberOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                            IndexSet * indexes);
        virtual IMAPFetchMessagesOperation * syncMessagesByUID(String * folder, IMAPMessagesRequestKind requestKind,
                                                               IndexSet * indexes, uint64_t modSeq);
        
		virtual IMAPFetchContentOperation * fetchMessageByUIDOperation(String * folder, uint32_t uid, bool urgent = false);
		virtual IMAPFetchContentOperation * fetchMessageAttachmentByUIDOperation(String * folder, uint32_t uid, String * partID,
                                                                                 Encoding encoding,
                                                                                 bool urgent = false);
		
		virtual IMAPOperation * storeFlagsOperation(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, MessageFlag flags);
		virtual IMAPOperation * storeLabelsOperation(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, Array * labels);
		
		virtual IMAPSearchOperation * searchOperation(String * folder, IMAPSearchKind kind, String * searchString);
		virtual IMAPSearchOperation * searchOperation(String * folder, IMAPSearchExpression * expression);
		
		virtual IMAPIdleOperation * idleOperation(String * folder, uint32_t lastKnownUID);
		
		virtual IMAPFetchNamespaceOperation * fetchNamespaceOperation();
		
		virtual IMAPIdentityOperation * identityOperation(String * vendor, String * name, String * version);
        
        virtual IMAPOperation * checkAccountOperation();
        
        virtual IMAPCapabilityOperation * capabilityOperation();
        
	private:
        Array * mSessions;
        
		String * mHostname;
		unsigned int mPort;
		String * mUsername;
		String * mPassword;
		AuthType mAuthType;
		ConnectionType mConnectionType;
		bool mCheckCertificateEnabled;
		bool mVoIPEnabled;
		char mDelimiter;
		IMAPNamespace * mDefaultNamespace;
		time_t mTimeout;
        bool mAllowsFolderConcurrentAccessEnabled;
		unsigned int mMaximumConnections;
        
        IMAPAsyncConnection * sessionForFolder(String * folder, bool urgent = false);
        IMAPAsyncConnection * session();
        IMAPAsyncConnection * matchingSessionForFolder(String * folder);
        IMAPAsyncConnection * availableSession();
        
	};
}

#endif

#endif /* defined(__mailcore2__MCIMAPAccount__) */
