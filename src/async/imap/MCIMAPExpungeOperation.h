//
//  MCIMAPExpungeOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPExpungeOperation__
#define __mailcore2__MCIMAPExpungeOperation__

#include <mailcore/MCIMAPOperation.h>

namespace mailcore {
    
    class IMAPExpungeOperation : public IMAPOperation {
    private:
        
    public:
        IMAPExpungeOperation();
        virtual ~IMAPExpungeOperation();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__MCIMAPExpungeOperation__) */
