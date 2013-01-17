//
//  MCPopAsyncSession.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCPopAsyncSession__
#define __mailcore2__MCPopAsyncSession__

#include <mailcore/MCBaseTypes.h>

namespace mailcore {
    
    class POPOperation;
    class POPSession;
    class POPFetchHeaderOperation;
    class POPFetchMessageOperation;
    class POPDeleteMessagesOperation;
    class POPFetchMessagesOperation;
	
	class POPAsyncSession : public Object {
	private:
        POPSession * mSession;
        OperationQueue * mQueue;
		
	public:
		POPAsyncSession();
		virtual ~POPAsyncSession();
		
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
        
		virtual POPFetchMessagesOperation * fetchMessagesOperation();
        
        virtual POPFetchHeaderOperation * fetchHeaderOperation(unsigned int index);
        
		virtual POPFetchMessageOperation * fetchMessage(unsigned int index);
        
        virtual POPOperation * deleteMessages(Array * indexes);
        
        // private
        virtual void runOperation(POPOperation * operation);
        virtual POPSession * session();
    };
}

#endif /* defined(__mailcore2__MCPopAsyncSession__) */
