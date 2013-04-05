//
//  SMTPSendWithDataOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCSMTPSENDWITHDATAOPERATION_H_

#define __MAILCORE_MCSMTPSENDWITHDATAOPERATION_H_

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCAbstract.h>
#include <MailCore/MCSMTPOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class SMTPSendWithDataOperation : public SMTPOperation {
    public:
        SMTPSendWithDataOperation();
        virtual ~SMTPSendWithDataOperation();
        
        virtual void setMessageData(Data * data);
        virtual Data * messageData();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        Data * mMessageData;
        
    };
}

#endif

#endif
