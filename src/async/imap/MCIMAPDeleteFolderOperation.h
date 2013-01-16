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
        String * mFolder;
        
    public:
        IMAPDeleteFolderOperation();
        virtual ~IMAPDeleteFolderOperation();
        
        virtual void setFolder(String * folder);
        virtual String * folder();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__MCIMAPDeleteFolderOperation__) */
