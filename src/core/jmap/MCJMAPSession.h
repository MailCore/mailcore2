#ifndef MAILCORE_MCJMAPSESSION_H

#define MAILCORE_MCJMAPSESSION_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

typedef struct mailjmap mailjmap;

namespace mailcore {

    class JMAPMailbox;
    class JMAPMessage;
    class JMAPBlobUpload;
    class JMAPSubmission;

    class MAILCORE_EXPORT JMAPSession : public Object {
    public:
        JMAPSession();
        virtual ~JMAPSession();

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

        virtual void connect(ErrorCode * pError);
        virtual void discover(ErrorCode * pError);
        virtual void login(ErrorCode * pError);
        virtual void disconnect();

        virtual String * primaryMailAccountID(ErrorCode * pError);

        virtual Array * /* JMAPMailbox */ fetchMailboxes(ErrorCode * pError);
        virtual Array * /* String */ queryMessagesWithText(String * text, unsigned int position, unsigned int limit, ErrorCode * pError);
        virtual Array * /* String */ queryMessagesInMailbox(String * mailboxID, unsigned int position, unsigned int limit, ErrorCode * pError);
        virtual Array * /* JMAPMessage */ fetchMessages(Array * ids, Array * properties, ErrorCode * pError);

        virtual JMAPBlobUpload * upload(Data * data, String * contentType, String * accountID, ErrorCode * pError);
        virtual Data * download(String * blobID, String * name, String * accept, String * accountID, ErrorCode * pError);

        virtual JMAPMessage * createDraft(Data * rfc822Data, String * mailboxID, Array * keywords, ErrorCode * pError);
        virtual void updateDraft(String * messageID, Array * mailboxIDs, Array * keywords, ErrorCode * pError);
        virtual void deleteDraft(String * messageID, ErrorCode * pError);
        virtual JMAPSubmission * sendMessage(String * messageID, String * identityID, ErrorCode * pError);
        virtual JMAPSubmission * sendData(Data * rfc822Data, String * identityID, ErrorCode * pError);

        virtual int lastHTTPStatus();
        virtual String * lastErrorMessage();
        virtual String * lastProblemType();
        virtual String * lastMethodErrorType();
        virtual String * lastMethodErrorDescription();

    public: // private
        virtual void setup(ErrorCode * pError);
        virtual void unsetup();
        virtual bool isSetup();

    private:
        String * mSessionURL;
        String * mDomainOrEmail;
        String * mAccountID;
        String * mUsername;
        String * mOAuth2Token;
        time_t mTimeout;
        bool mCheckCertificateEnabled;
        mailjmap * mJMAP;

        void init();
        String * effectiveAccountID(ErrorCode * pError);
        bool checkCertificate();
    };

}

#endif

#endif
