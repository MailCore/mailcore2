//
//  SMTPSendWithRecipientOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCSMTPSendWithRecipientOperation__
#define __mailcore2__MCSMTPSendWithRecipientOperation__

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstract.h>
#include <mailcore/MCSMTPOperation.h>

namespace mailcore {
    
    class SMTPSendWithRecipientOperation : public SMTPOperation {
    private:
        Array * mRecipients;
        Address * mFrom;
        Data * mMessageData;
        
        void init();
        
    public:
        SMTPSendWithRecipientOperation();
        virtual ~SMTPSendWithRecipientOperation();
        
        virtual void setFrom(Address * from);
        virtual Address * from();
        
        virtual void setRecipients(Array * recipients);
        virtual Array * recipients();
        
        virtual void setMessageData(Data * data);
        virtual Data * messageData();
        
        virtual void main();
    };
    
}

#endif /* defined(__mailcore2__SMTPSendWithRecipientOperation__) */
