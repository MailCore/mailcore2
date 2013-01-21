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

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPExpungeOperation : public IMAPOperation {
    public:
        IMAPExpungeOperation();
        virtual ~IMAPExpungeOperation();
        
    public: // subclass behavior
        virtual void main();
    };
}

#endif

#endif /* defined(__mailcore2__MCIMAPExpungeOperation__) */
