//
//  MCIMAPDeleteFolderOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPDeleteFolderOperation__
#define __mailcore2__MCIMAPDeleteFolderOperation__

#include <mailcore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPDeleteFolderOperation : public IMAPOperation {
    public:
        IMAPDeleteFolderOperation();
        virtual ~IMAPDeleteFolderOperation();
        
    public: // subclass behavior
        virtual void main();
    };
}

#endif

#endif /* defined(__mailcore2__MCIMAPDeleteFolderOperation__) */
