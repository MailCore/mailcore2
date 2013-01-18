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

namespace mailcore {
    
    class IMAPDeleteFolderOperation : public IMAPOperation {
    private:
        
    public:
        IMAPDeleteFolderOperation();
        virtual ~IMAPDeleteFolderOperation();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__MCIMAPDeleteFolderOperation__) */
