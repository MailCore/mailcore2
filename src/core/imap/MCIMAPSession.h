#ifndef __MAILCORE_MCIMAPSESSION_H

#define __MAILCORE_MCIMAPSESSION_H

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCMessageConstants.h>
#include <libetpan/libetpan.h>

namespace mailcore {

	extern String * IMAPNamespacePersonal;
	extern String * IMAPNamespaceOther;
	extern String * IMAPNamespaceShared;

	class IMAPNamespace;
	class IMAPSearchExpression;
	class IMAPFolder;
	class IMAPProgressCallback;
	
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
		
		bool mIdleEnabled;
		bool mXListEnabled;
		String * mWelcomeString;
		bool mNeedsMboxMailWorkaround;
		time_t mTimeout;
		uint32_t mUIDValidity;
		uint32_t mUIDNext;
		unsigned int mFolderMsgCount;
		unsigned int mLastFetchedSequenceNumber;
		String * mCurrentFolder;
		pthread_mutex_t mIdleLock;
		int mState;
		mailimap * mImap;
		IMAPProgressCallback * mProgressCallback;
		unsigned int mProgressItemsCount;
		
		void init();
		void bodyProgress(unsigned int current, unsigned int maximum);
		void itemsProgress(unsigned int current, unsigned int maximum);
		bool checkCertificate();
		static void body_progress(size_t current, size_t maximum, void * context);
		static void items_progress(size_t current, size_t maximum, void * context);
		void setup();
		void unsetup();
		void connectIfNeeded(ErrorCode * pError);
		void loginIfNeeded(ErrorCode * pError);
		void selectIfNeeded(String * folder, ErrorCode * pError);
		char fetchDelimiterIfNeeded(char defaultDelimiter, ErrorCode * pError);
		Array * fetchMessages(String * folder, IMAPMessagesRequestKind requestKind, bool fetchByUID,
			struct mailimap_set * imapset, HashMap * mapping, uint32_t startUid,
			IMAPProgressCallback * progressCallback, ErrorCode * pError);
		
	public:
		IMAPSession();
		virtual ~IMAPSession();
		
		//virtual String * className();
		
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

		virtual void select(String * folder, ErrorCode * pError);
		
		virtual Array * /* IMAPFolder */ fetchSubscribedFolders(ErrorCode * pError);
		virtual Array * /* IMAPFolder */ fetchAllFolders(ErrorCode * pError); // will use xlist if available

		virtual void renameFolder(String * folder, String * otherName, ErrorCode * pError);
		virtual void deleteFolder(String * folder, ErrorCode * pError);
		virtual void createFolder(String * folder, ErrorCode * pError);

		virtual void subscribeFolder(String * folder, ErrorCode * pError);
		virtual void unsubscribeFolder(String * folder, ErrorCode * pError);

		virtual void appendMessage(String * folder, Data * messageData, MessageFlag flags,
			IMAPProgressCallback * progressCallback, uint32_t * createdUID, ErrorCode * pError);
		
		virtual void copyMessages(String * folder, Array * uidSet, String * destFolder,
			 Array ** pDestUIDs, ErrorCode * pError);
		
		virtual void expunge(String * folder, ErrorCode * pError);
		
		virtual Array * fetchMessagesByUID(String * folder, IMAPMessagesRequestKind requestKind,
			uint32_t firstUID, uint32_t lastUID, IMAPProgressCallback * progressCallback, ErrorCode * pError);
		virtual Array * fetchMessagesByNumber(String * folder, IMAPMessagesRequestKind requestKind,
			uint32_t firstNumber, uint32_t lastNumber, IMAPProgressCallback * progressCallback, ErrorCode * pError);
		virtual Array * fetchMessagesByUID(String * folder, IMAPMessagesRequestKind requestKind,
			Array * numbers, IMAPProgressCallback * progressCallback, ErrorCode * pError);
		virtual Array * fetchMessagesByNumber(String * folder, IMAPMessagesRequestKind requestKind,
			Array * numbers, IMAPProgressCallback * progressCallback, ErrorCode * pError);
		virtual Data * fetchMessageByUID(String * folder, uint32_t uid,
			IMAPProgressCallback * progressCallback, ErrorCode * pError);
		virtual Data * fetchMessageAttachmentByUID(String * folder, uint32_t uid, String * partID,
		    Encoding encoding, unsigned int expectedSize,
			IMAPProgressCallback * progressCallback, ErrorCode * pError);
		virtual HashMap * fetchMessageNumberUIDMapping(String * folder, uint32_t fromUID, uint32_t toUID,
			ErrorCode * pError);
		
		virtual void storeFlags(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, MessageFlag flags, ErrorCode * pError);
		virtual void storeLabels(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, Array * labels, ErrorCode * pError);
		
		virtual Array * search(String * folder, IMAPSearchKind kind, String * searchString, ErrorCode * pError);
		virtual Array * search(String * folder, IMAPSearchExpression * expression, ErrorCode * pError);
		
		virtual void setupIdle();
		virtual void idle(String * folder, uint32_t lastKnownUID, ErrorCode * pError);
		virtual void interruptIdle();
		virtual void unsetupIdle();
		
		virtual void connect(ErrorCode * pError);
		virtual void disconnect();
		
		virtual HashMap * fetchNamespace(ErrorCode * pError);
		
		virtual void login(ErrorCode * pError);
		
		virtual HashMap * identity(String * vendor, String * name, String * version, ErrorCode * pError);
	};
}

#endif
