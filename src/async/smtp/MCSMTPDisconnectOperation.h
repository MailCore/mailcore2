//
//  SMTPDisconnectOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 6/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCSMTPDISCONNECTOPERATION_H_
#define __MAILCORE_MCSMTPDISCONNECTOPERATION_H_

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCAbstract.h>
#include <MailCore/MCSMTPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class SMTPDisconnectOperation : public SMTPOperation {
    public:
        SMTPDisconnectOperation();
        virtual ~SMTPDisconnectOperation();
        
    public: // subclass behavior
        virtual void main();
    };
}

#endif

#endif