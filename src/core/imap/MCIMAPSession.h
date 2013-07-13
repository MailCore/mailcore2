#ifndef __MAILCORE_MCIMAPSESSION_H

#define __MAILCORE_MCIMAPSESSION_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>
#include <MailCore/MCIMAPMessage.h>

#ifdef __cplusplus

namespace mailcore {
    
    extern String * IMAPNamespacePersonal;
    extern String * IMAPNamespaceOther;
    extern String * IMAPNamespaceShared;
    
    class IMAPNamespace;
    class IMAPSearchExpression;
    class IMAPFolder;
    class IMAPProgressCallback;
    class IMAPSyncResult;
    class IMAPFolderStatus;
    
    class IMAPSession : public Object {
    public:
        IMAPSession();
        virtual ~IMAPSession();
        
        virtual void setHostname(String * hostname);
        virtual String * hostname();
        
        virtual void setPort(unsigned int port);
        virtual unsigned int port();
        
        virtual void setUsername(String * username);
        virtual String * username();
        
        virtual void setPassword(String * password);
        virtual String * password();
        
        // To authenticate using OAuth2, username and oauth2token should be set.
        // auth type to use is AuthTypeOAuth2.
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
        
        // Needed for fetchSubscribedFolders() and fetchAllFolders().
        virtual void setDelimiter(char delimiter);
        virtual char delimiter();
        
        // Needed for fetchSubscribedFolders() and fetchAllFolders().
        virtual void setDefaultNamespace(IMAPNamespace * ns);
        virtual IMAPNamespace * defaultNamespace();
        
        virtual void select(String * folder, ErrorCode * pError);
        virtual IMAPFolderStatus * folderStatus(String * folder, ErrorCode * pError);
        
        virtual Array * /* IMAPFolder */ fetchSubscribedFolders(ErrorCode * pError);
        virtual Array * /* IMAPFolder */ fetchAllFolders(ErrorCode * pError); // will use xlist if available
        
        virtual void renameFolder(String * folder, String * otherName, ErrorCode * pError);
        virtual void deleteFolder(String * folder, ErrorCode * pError);
        virtual void createFolder(String * folder, ErrorCode * pError);
        
        virtual void subscribeFolder(String * folder, ErrorCode * pError);
        virtual void unsubscribeFolder(String * folder, ErrorCode * pError);
        
        virtual void appendMessage(String * folder, Data * messageData, MessageFlag flags,
                                   IMAPProgressCallback * progressCallback, uint32_t * createdUID, ErrorCode * pError);
        
        virtual void copyMessages(String * folder, IndexSet * uidSet, String * destFolder,
                                  IndexSet ** pDestUIDs, ErrorCode * pError);
        
        virtual void expunge(String * folder, ErrorCode * pError);
        
        virtual Array * /* IMAPMessage */ fetchMessagesByUID(String * folder, IMAPMessagesRequestKind requestKind,
                                                             IndexSet * uids, IMAPProgressCallback * progressCallback, ErrorCode * pError);
        virtual Array * /* IMAPMessage */ fetchMessagesByNumber(String * folder, IMAPMessagesRequestKind requestKind,
                                                                IndexSet * numbers, IMAPProgressCallback * progressCallback, ErrorCode * pError);
        
        virtual Data * fetchMessageByUID(String * folder, uint32_t uid,
                                         IMAPProgressCallback * progressCallback, ErrorCode * pError);
        virtual Data * fetchMessageAttachmentByUID(String * folder, uint32_t uid, String * partID,
                                                   Encoding encoding, IMAPProgressCallback * progressCallback, ErrorCode * pError);
        virtual HashMap * fetchMessageNumberUIDMapping(String * folder, uint32_t fromUID, uint32_t toUID,
                                                       ErrorCode * pError);
        
        /* When CONDSTORE or QRESYNC is available */
        virtual IMAPSyncResult * syncMessagesByUID(String * folder, IMAPMessagesRequestKind requestKind,
                                                   IndexSet * uids, uint64_t modseq,
                                                   IMAPProgressCallback * progressCallback, ErrorCode * pError);
        
        virtual void storeFlags(String * folder, IndexSet * uids, IMAPStoreFlagsRequestKind kind, MessageFlag flags, ErrorCode * pError);
        virtual void storeLabels(String * folder, IndexSet * uids, IMAPStoreFlagsRequestKind kind, Array * labels, ErrorCode * pError);
        
        virtual IndexSet * search(String * folder, IMAPSearchKind kind, String * searchString, ErrorCode * pError);
        virtual IndexSet * search(String * folder, IMAPSearchExpression * expression, ErrorCode * pError);
        
        virtual bool setupIdle();
        virtual void idle(String * folder, uint32_t lastKnownUID, ErrorCode * pError);
        virtual void interruptIdle();
        virtual void unsetupIdle();
        
        virtual void connect(ErrorCode * pError);
        virtual void disconnect();
        
        virtual HashMap * fetchNamespace(ErrorCode * pError);
        
        virtual void login(ErrorCode * pError);
        
        virtual HashMap * identity(String * vendor, String * name, String * version, ErrorCode * pError);
        
        virtual IndexSet * capability(ErrorCode * pError);
        
        virtual uint32_t uidValidity();
        virtual uint32_t uidNext();
        virtual uint64_t modSequenceValue();
        virtual unsigned int lastFolderMessageCount();
        virtual uint32_t firstUnseenUid();
        
        virtual bool isIdleEnabled();
        virtual bool isXListEnabled();
        virtual bool isCondstoreEnabled();
        virtual bool isQResyncEnabled();
        virtual bool isIdentityEnabled();
        
        virtual void setConnectionLogger(ConnectionLogger * logger);
        virtual ConnectionLogger * connectionLogger();
        
        /** HTML rendering of the body of the message to be displayed in a web view.*/
        virtual String * htmlRendering(IMAPMessage * message, String * folder, ErrorCode * pError);
        
        /** HTML rendering of the body of the message.*/
        virtual String * htmlBodyRendering(IMAPMessage * message, String * folder, ErrorCode * pError);
        
        /** Text rendering of the message.*/
        virtual String * plainTextRendering(IMAPMessage * message, String * folder, ErrorCode * pError);
        
        /** Text rendering of the body of the message. All end of line will be removed and white spaces cleaned up.
         This method can be used to generate the summary of the message.*/
        virtual String * plainTextBodyRendering(IMAPMessage * message, String * folder, ErrorCode * pError);
        
    public: // private
        virtual void loginIfNeeded(ErrorCode * pError);
        virtual void connectIfNeeded(ErrorCode * pError);
        virtual bool isDisconnected();
        
    private:
        String * mHostname;
        unsigned int mPort;
        String * mUsername;
        String * mPassword;
        String * mOAuth2Token;
        AuthType mAuthType;
        ConnectionType mConnectionType;
        bool mCheckCertificateEnabled;
        bool mVoIPEnabled;
        char mDelimiter;
        IMAPNamespace * mDefaultNamespace;
        time_t mTimeout;
        
        bool mBodyProgressEnabled;
        bool mIdleEnabled;
        bool mXListEnabled;
        bool mCondstoreEnabled;
        bool mQResyncEnabled;
        bool mIdentityEnabled;
        String * mWelcomeString;
        bool mNeedsMboxMailWorkaround;
        uint32_t mUIDValidity;
        uint32_t mUIDNext;
        uint64_t mModSequenceValue;
        unsigned int mFolderMsgCount;
        uint32_t mFirstUnseenUid;
        
        unsigned int mLastFetchedSequenceNumber;
        String * mCurrentFolder;
        pthread_mutex_t mIdleLock;
        bool mCanIdle;
        int mState;
        mailimap * mImap;
        IMAPProgressCallback * mProgressCallback;
        unsigned int mProgressItemsCount;
        ConnectionLogger * mConnectionLogger;
        
        void init();
        void bodyProgress(unsigned int current, unsigned int maximum);
        void itemsProgress(unsigned int current, unsigned int maximum);
        bool checkCertificate();
        static void body_progress(size_t current, size_t maximum, void * context);
        static void items_progress(size_t current, size_t maximum, void * context);
        void setup();
        void unsetup();
        void selectIfNeeded(String * folder, ErrorCode * pError);
        char fetchDelimiterIfNeeded(char defaultDelimiter, ErrorCode * pError);
        IMAPSyncResult * fetchMessages(String * folder, IMAPMessagesRequestKind requestKind, bool fetchByUID,
                                       struct mailimap_set * imapset, uint64_t modseq, HashMap * mapping, uint32_t startUid,
                                       IMAPProgressCallback * progressCallback, ErrorCode * pError);
    };
}

#endif

#endif
