#ifndef __MAILCORE_MCSMTPASYNCSESSION_H

#define __MAILCORE_MCSMTPASYNCSESSION_H

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCMessageConstants.h>
#include <libetpan/libetpan.h>

namespace mailcore {
	
	class MessageBuilder;
	class SMTPOperation;
	class SMTPSession;
    class Address;

	class SMTPAsyncSession : public Object {
	private:
		String * mHostname;
		unsigned int mPort;
		String * mUsername;
		String * mPassword;
		AuthType mAuthType;
		ConnectionType mConnectionType;
		time_t mTimeout;
		bool mCheckCertificateEnabled;
		bool mUseHeloIPEnabled;
		
		SMTPSession * mSession;
		OperationQueue * mQueue;
		
		void queue(SMTPOperation * op);
		
	public:
		SMTPAsyncSession();
		virtual ~SMTPAsyncSession();
		
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
		
		virtual void setUseHeloIPEnabled(bool enabled);
		virtual bool useHeloIPEnabled();
		
		virtual SMTPOperation * sendMessageOperationWithFromAndRecipient(Address * from, Array * recipients, Data * messageData);
		virtual SMTPOperation * sendMessageOperation(Data * messageData);
		virtual SMTPOperation * sendMessageOperation(MessageBuilder * msg);
        virtual SMTPOperation * checkAccountOperation(Address * from);
        
        // private
        virtual void runOperation(SMTPOperation * operation);
        virtual SMTPSession * session();
	};
	
}

#endif
