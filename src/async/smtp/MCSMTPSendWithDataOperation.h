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

#endif /* defined(__mailcore2__SMTPSendWithDataOperation__) */
