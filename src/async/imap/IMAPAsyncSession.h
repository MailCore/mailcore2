#ifndef __MAILCORE_MCIMAPASYNCSESSION_H

#define __MAILCORE_MCIMAPASYNCSESSION_H

#include <mailcore/CBaseTypes.h>
#include <mailcore/MessageConstants.h>

namespace mailcore {
	
	class IMAPOperation;
	class IMAPFetchFoldersOperation;
	class IMAPAppendOperation;
	class IMAPCopyOperation;
	class IMAPFetchMessagesOperation;
	class IMAPIdleOperation;
	class IMAPSession;
	class IMAPNamespace;

	class IMAPSession : public Object {
	private:
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
		
		IMAPSession * mSession;
		
	public:
		IMAPAsyncSession();
		virtual ~IMAPAsyncSession();
		
		virtual String * className();
		
		virtual void setHostname(String * hostname);
		virtual String * hostname();

		virtual void setPort(unsigned int port);
		virtual unsigned int port();

		virtual void setUsername(String * login);
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

		virtual IMAPOperation * select(String * folder);
		
		virtual IMAPFetchFoldersOperation * fetchSubscribedFolders();
		virtual IMAPFetchFoldersOperation * fetchAllFolders();

		virtual IMAPOperation * renameFolder(String * folder, String * otherName);
		virtual IMAPOperation * deleteFolder(String * folder, ErrorCode * pError);
		virtual IMAPOperation * createFolder(String * folder, ErrorCode * pError);

		virtual IMAPOperation * subscribeFolder(String * folder, ErrorCode * pError);
		virtual IMAPOperation * unsubscribeFolder(String * folder, ErrorCode * pError);

		virtual IMAPAppendOperation * appendMessage(String * folder, Data * messageData, MessageFlag flags);
		
		virtual IMAPCopyOperation * copyMessages(String * folder, Array * uidSet, String * destFolder);
		
		virtual IMAPOperation * expunge(String * folder);
		
		virtual IMAPFetchMessagesOperation * fetchMessagesByUID(String * folder, IMAPMessagesRequestKind requestKind,
			uint32_t firstUID, uint32_t lastUID);
		virtual IMAPFetchMessagesOperation * fetchMessagesByNumber(String * folder, IMAPMessagesRequestKind requestKind,
			uint32_t firstNumber, uint32_t lastNumber);
		virtual IMAPFetchMessagesOperation * fetchMessagesByUID(String * folder, IMAPMessagesRequestKind requestKind,
			Array * numbers);
		virtual IMAPFetchMessagesOperation * fetchMessagesByNumber(String * folder, IMAPMessagesRequestKind requestKind,
			Array * numbers);
		virtual IMAPFetchMessageOperation * fetchMessageByUID(String * folder, uint32_t uid);
		virtual IMAPFetchMessageOperation * fetchMessageAttachmentByUID(String * folder, uint32_t uid, String * partID,
		    Encoding encoding, unsigned int expectedSize);
		
		virtual IMAPOperation * storeFlags(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, MessageFlag flags);
		virtual IMAPOperation * storeLabels(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, Array * labels);
		
		virtual IMAPSearchOperation * search(String * folder, IMAPSearchKind kind, String * searchString);
		virtual IMAPSearchOperation * search(String * folder, IMAPSearchExpression * expression);
		
		virtual IMAPIdleOperation * idle(String * folder, uint32_t lastKnownUID);
		
		virtual IMAPOperation * connect();
		virtual IMAPOperation * disconnect();
		
		virtual IMAPOperation * fetchNamespace();
		
		virtual IMAPOperation * login();
		
		virtual IMAPOperation * identity(String * vendor, String * name, String * version);
	};
}

#endif
