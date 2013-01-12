//
//  MCSMTPOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/11/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCSMTPOperation__
#define __mailcore2__MCSMTPOperation__

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCSMTPProgressCallback.h>

namespace mailcore {

    class SMTPAsyncSession;
    class SMTPOperationCallback;
    
    class SMTPOperation : public Operation, public SMTPProgressCallback {
        SMTPAsyncSession * mSession;
        SMTPOperationCallback * mSmtpCallback;
        ErrorCode mError;
    private:
		virtual void bodyProgress(SMTPSession * session, unsigned int current, unsigned int maximum);
        virtual void bodyProgressOnMainThread(void * context);
        
    public:
        SMTPOperation();
        virtual ~SMTPOperation();
        
        virtual void setSession(SMTPAsyncSession * session);
        virtual SMTPAsyncSession * session();
        
        virtual void setSmtpCallback(SMTPOperationCallback * callback);
        virtual SMTPOperationCallback * smtpCallback();
        
        virtual void setError(ErrorCode error);
        virtual ErrorCode error();
        
        virtual void start();
    };
    
}

#endif /* defined(__mailcore2__MCSMTPOperation__) */
