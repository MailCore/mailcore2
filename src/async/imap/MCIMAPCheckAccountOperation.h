//
//  MCIMAPCheckAccountOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPCheckAccountOperation__
#define __mailcore2__MCIMAPCheckAccountOperation__

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPCheckAccountOperation : public IMAPOperation {
    public: // subclass behavior
        virtual void main();
    };
}

#endif

#endif /* defined(__mailcore2__MCIMAPCheckAccountOperation__) */
