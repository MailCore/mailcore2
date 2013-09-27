//
//  MCPopAsyncSession.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCPOPASYNCSESSION_H_

#define __MAILCORE_MCPOPASYNCSESSION_H_

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {
    
    class POPOperation;
    class POPSession;
    class POPFetchHeaderOperation;
    class POPFetchMessageOperation;
    class POPDeleteMessagesOperation;
    class POPFetchMessagesOperation;
    class POPOperationQueueCallback;
    class POPConnectionLogger;
    
    class POPAsyncSession : public Object {
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
        
        virtual void setConnectionLogger(ConnectionLogger * logger);
        virtual ConnectionLogger * connectionLogger();
        
        virtual POPFetchMessagesOperation * fetchMessagesOperation();
        
        virtual POPFetchHeaderOperation * fetchHeaderOperation(unsigned int index);
        
        virtual POPFetchMessageOperation * fetchMessageOperation(unsigned int index);
        
        // Will disconnect.
        virtual POPOperation * deleteMessagesOperation(IndexSet * indexes);
        
        virtual POPOperation * disconnectOperation();
        
        virtual POPOperation * checkAccountOperation();
        
        virtual POPOperation * noopOperation();
        
    private:
        POPSession * mSession;
        OperationQueue * mQueue;
        POPOperationQueueCallback * mQueueCallback;
        ConnectionLogger * mConnectionLogger;
        pthread_mutex_t mConnectionLoggerLock;
        POPConnectionLogger * mInternalLogger;
        
    public: // private
        virtual void runOperation(POPOperation * operation);
        virtual POPSession * session();
        virtual void logConnection(ConnectionLogType logType, Data * buffer);
    };
}

#endif

#endif
