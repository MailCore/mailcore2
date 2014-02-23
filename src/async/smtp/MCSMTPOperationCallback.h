//
//  MCSMTPOperationCallback.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/11/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCSMTPOPERATIONCALLBACK_H

#define MAILCORE_MCSMTPOPERATIONCALLBACK_H

#ifdef __cplusplus

namespace mailcore {
    
    class SMTPOperation;
    
    class SMTPOperationCallback {
    public:
        virtual void bodyProgress(SMTPOperation * session, unsigned int current, unsigned int maximum) {};
    };
    
}

#endif

#endif
