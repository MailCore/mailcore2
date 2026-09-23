#ifndef MAILCORE_MCGMAILSESSION_H

#define MAILCORE_MCGMAILSESSION_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>
#include <MailCore/MCGmailTypes.h>

#ifdef __cplusplus

typedef struct mailgmail mailgmail;

namespace mailcore {

    class GmailProfile;
    class GmailLabel;
    class GmailMessageList;
    class GmailMessage;
    class GmailMessagePart;
    class AbstractPart;

    class MAILCORE_EXPORT GmailSession : public Object {
    public:
        GmailSession();
        virtual ~GmailSession();

        virtual void setUserID(String * userID);
        virtual String * userID();

        virtual void setOAuth2Token(String * token);
        virtual String * OAuth2Token();

        virtual void setUserAgent(String * userAgent);
        virtual String * userAgent();

        virtual void setTimeout(time_t timeout);
        virtual time_t timeout();

        virtual GmailProfile * profile(ErrorCode * pError);
        virtual Array * /* GmailLabel */ labels(ErrorCode * pError);
        virtual GmailLabel * label(String * labelID, ErrorCode * pError);

        virtual GmailMessageList * messages(ErrorCode * pError);
        virtual GmailMessageList * messagesWithQuery(String * query, ErrorCode * pError);
        virtual GmailMessageList * messagesWithLabel(String * labelID, ErrorCode * pError);

        virtual GmailMessage * message(String * messageID, ErrorCode * pError);
        virtual GmailMessage * messageWithFormat(String * messageID, GmailMessageFormat format,
                                                 ErrorCode * pError);
        virtual GmailMessage * messageWithMetadataHeaders(String * messageID, Array * headers,
                                                          ErrorCode * pError);
        virtual Data * messageData(String * messageID, ErrorCode * pError);

        virtual Data * attachmentData(String * messageID, String * attachmentID,
                                      ErrorCode * pError);
        virtual Data * dataForPart(String * messageID, AbstractPart * part,
                                   ErrorCode * pError);

        virtual int lastHTTPStatus();
        virtual String * lastErrorMessage();

    public: // private
        virtual void setup(ErrorCode * pError);
        virtual void unsetup();
        virtual bool isSetup();

    private:
        String * mUserID;
        String * mOAuth2Token;
        String * mUserAgent;
        time_t mTimeout;
        mailgmail * mGmail;

        void init();
        void applyConfiguration(ErrorCode * pError);
    };

}

#endif

#endif
