//
//  SMTPSendWithDataOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCSMTPSendWithDataOperation__
#define __mailcore2__MCSMTPSendWithDataOperation__

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstract.h>
#include <mailcore/MCSMTPOperation.h>

namespace mailcore {

    class SMTPSendWithDataOperation : public SMTPOperation {
    private:
        Data * mMessageData;
        
    public:
        SMTPSendWithDataOperation();
        virtual ~SMTPSendWithDataOperation();
        
        virtual void setMessageData(Data * data);
        virtual Data * messageData();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__SMTPSendWithDataOperation__) */
