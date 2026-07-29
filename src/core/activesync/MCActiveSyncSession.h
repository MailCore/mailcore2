#ifndef MAILCORE_MCACTIVESYNCSESSION_H

#define MAILCORE_MCACTIVESYNCSESSION_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncOptions.h>
#include <MailCore/MCActiveSyncFolderSyncResult.h>
#include <MailCore/MCActiveSyncSyncRequest.h>
#include <MailCore/MCActiveSyncSyncResult.h>
#include <MailCore/MCActiveSyncProvisionResult.h>
#include <MailCore/MCActiveSyncItemEstimateResult.h>
#include <MailCore/MCActiveSyncMessage.h>
#include <MailCore/MCActiveSyncPingResult.h>

#ifdef __cplusplus

struct mailactivesync;

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncSession : public Object {
    public:
        ActiveSyncSession();
        virtual ~ActiveSyncSession();

        virtual void setServerURL(String * serverURL);
        virtual String * serverURL();
        virtual void setUsername(String * username);
        virtual String * username();
        virtual void setPassword(String * password);
        virtual String * password();
        virtual void setOAuth2Token(String * token);
        virtual String * OAuth2Token();
        virtual void setDeviceID(String * deviceID);
        virtual String * deviceID();

        virtual String * lastRedirectURL();
        virtual String * lastAuthenticateHeader();

        virtual void connect(ErrorCode * pError);
        virtual void login(ErrorCode * pError);
        virtual void loginOAuth2(ErrorCode * pError);
        virtual void setOAuth2TokenOnConnection(ErrorCode * pError);
        virtual ActiveSyncOptions * options(ErrorCode * pError);
        virtual ActiveSyncFolderSyncResult * folderSync(String * syncKey, ErrorCode * pError);
        virtual ActiveSyncSyncResult * sync(ActiveSyncSyncRequest * request, ErrorCode * pError);
        virtual ActiveSyncSyncResult * syncMessages(String * folderID, String * syncKey, ErrorCode * pError);
        virtual ActiveSyncProvisionResult * provision(ErrorCode * pError);
        virtual ActiveSyncItemEstimateResult * itemEstimate(String * collectionID, String * syncKey, ErrorCode * pError);
        virtual ActiveSyncMessage * fetchMessage(String * folderID, String * messageID, ErrorCode * pError);
        virtual void sendMessage(Data * messageData, bool saveInSent, ErrorCode * pError);
        virtual void smartReply(String * folderID, String * messageID, Data * messageData, bool saveInSent, ErrorCode * pError);
        virtual void smartForward(String * folderID, String * messageID, Data * messageData, bool saveInSent, ErrorCode * pError);
        virtual ActiveSyncPingResult * ping(Array * /* String */ collectionIDs, uint32_t heartbeatInterval, ErrorCode * pError);

    private:
        String * mServerURL;
        String * mUsername;
        String * mPassword;
        String * mOAuth2Token;
        String * mDeviceID;
        String * mProtocolVersion;
        String * mPolicyKey;
        String * mUserAgent;
        mailactivesync * mSession;
        void init();
        void ensureSession();
        void configureSession(ErrorCode * pError);
    };

}

#endif

#endif
