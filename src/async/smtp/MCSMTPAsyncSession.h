#ifndef __MAILCORE_MCSMTPASYNCSESSION_H

#define __MAILCORE_MCSMTPASYNCSESSION_H

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCMessageConstants.h>
#include <libetpan/libetpan.h>

#ifdef __cplusplus

namespace mailcore {
	
	class MessageBuilder;
	class SMTPOperation;
	class SMTPSession;
    class Address;

	class SMTPAsyncSession : public Object {
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
		
		virtual SMTPOperation * sendMessageOperation(Data * messageData);
        virtual SMTPOperation * checkAccountOperation(Address * from);
        
    public: // private
        virtual void runOperation(SMTPOperation * operation);
        virtual SMTPSession * session();
        
	private:
		SMTPSession * mSession;
		OperationQueue * mQueue;
		
		void queue(SMTPOperation * op);
		
	};
	
}

#endif

#endif
