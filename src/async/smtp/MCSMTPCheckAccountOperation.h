//
//  MCSMTPCheckAccountOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCSMTPCHECKACCOUNTOPERATION_H_

#define __MAILCORE_MCSMTPCHECKACCOUNTOPERATION_H_

#include <MailCore/MCSMTPOperation.h>
#include <MailCore/MCAbstract.h>

#ifdef __cplusplus

namespace mailcore {
    
    class SMTPCheckAccountOperation : public SMTPOperation {
    public:
        SMTPCheckAccountOperation();
        virtual ~SMTPCheckAccountOperation();
        
        virtual void setFrom(Address * from);
        virtual Address * from();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        Address * mFrom;
        
    };

}

#endif

#endif
