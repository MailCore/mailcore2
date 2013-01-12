//
//  MCSMTPCheckAccountOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCSMTPCheckAccountOperation__
#define __mailcore2__MCSMTPCheckAccountOperation__

#include <mailcore/MCSMTPOperation.h>
#include <mailcore/MCAbstract.h>

namespace mailcore {
    
    class SMTPCheckAccountOperation : public SMTPOperation {
    private:
        Address * mFrom;
        
    public:
        SMTPCheckAccountOperation();
        virtual ~SMTPCheckAccountOperation();
        
        virtual void setFrom(Address * from);
        virtual Address * from();
        
        virtual void main();
    };

}

#endif /* defined(__mailcore2__MCSMTPCheckAccountOperation__) */
