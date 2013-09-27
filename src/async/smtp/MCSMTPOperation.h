//
//  MCSMTPOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/11/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCSMTPOPERATION_H_

#define __MAILCORE_MCSMTPOPERATION_H_

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCSMTPProgressCallback.h>

#ifdef __cplusplus

namespace mailcore {

    class SMTPAsyncSession;
    class SMTPOperationCallback;
    
    class SMTPOperation : public Operation, public SMTPProgressCallback {
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
        
    private:
        SMTPAsyncSession * mSession;
        SMTPOperationCallback * mSmtpCallback;
        ErrorCode mError;
    private:
        virtual void bodyProgress(SMTPSession * session, unsigned int current, unsigned int maximum);
        virtual void bodyProgressOnMainThread(void * context);
    };
    
}

#endif

#endif
