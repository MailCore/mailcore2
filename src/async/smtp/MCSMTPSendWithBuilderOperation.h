//
//  SMTPSendWithBuilderOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCSMTPSendWithBuilderOperation__
#define __mailcore2__MCSMTPSendWithBuilderOperation__

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstract.h>
#include <mailcore/MCSMTPOperation.h>

namespace mailcore {

    class MessageBuilder;
    
    class SMTPSendWithBuilderOperation : public SMTPOperation {
        MessageBuilder * mBuilder;
    private:
        
    public:
        SMTPSendWithBuilderOperation();
        virtual ~SMTPSendWithBuilderOperation();
        
        virtual void setBuilder(MessageBuilder * builder);
        virtual MessageBuilder * builder();
        
        void main();
    };
    
}

#endif /* defined(__mailcore2__SMTPSendWithBuilderOperation__) */
