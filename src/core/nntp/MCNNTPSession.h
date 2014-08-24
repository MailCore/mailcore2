#ifndef MAILCORE_MCNNTPSESSION_H
#define MAILCORE_MCNNTPSESSION_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {
    
    class NNTPProgressCallback;
    class MessageHeader;
    
    class NNTPSession : public Object {
    public:
        NNTPSession();
        virtual ~NNTPSession();
        
        virtual void setHostname(String * hostname);
        virtual String * hostname();
        
        virtual void setPort(unsigned int port);
        virtual unsigned int port();
        
        virtual void setUsername(String * username);
        virtual String * username();
        
        virtual void setPassword(String * password);
        virtual String * password();
        
        virtual void setConnectionType(ConnectionType connectionType);
        virtual ConnectionType connectionType();
        
        virtual void setTimeout(time_t timeout);
        virtual time_t timeout();
        
        virtual void setCheckCertificateEnabled(bool enabled);
        virtual bool isCheckCertificateEnabled();
        
        virtual void connect(ErrorCode * pError);
        virtual void disconnect();
        
        virtual void login(ErrorCode * pError);
        
        virtual void checkAccount(ErrorCode * pError);
        
        virtual Array * /* NNTPGroupInfo */ listAllNewsgroups(ErrorCode * pError);
        virtual Array * listSubscribedNewsgroups(ErrorCode * pError);
        
        virtual MessageHeader * fetchHeader(String *groupName, unsigned int index, ErrorCode * pError);

        virtual IndexSet * fetchArticles(String * groupname, ErrorCode * pError);
        
        Data * fetchArticle(String *groupName, unsigned int index, NNTPProgressCallback * callback, ErrorCode * pError);
        
        virtual void setConnectionLogger(ConnectionLogger * logger);
        virtual ConnectionLogger * connectionLogger();
        
    private:
        String * mHostname;
        unsigned int mPort;
        String * mUsername;
        String * mPassword;
        ConnectionType mConnectionType;
        bool mCheckCertificateEnabled;
        time_t mTimeout;
        
        newsnntp * mNNTP;
        int mState;
        
        ConnectionLogger * mConnectionLogger;
        
        void init();
        bool checkCertificate();
        void setup();
        void unsetup();
        void connectIfNeeded(ErrorCode * pError);
        void loginIfNeeded(ErrorCode * pError);
        void readerIfNeeded(ErrorCode * pError);
        void listIfNeeded(ErrorCode * pError);
        void selectGroup(String * folder, ErrorCode * pError);
    };
	
}

#endif

#endif
