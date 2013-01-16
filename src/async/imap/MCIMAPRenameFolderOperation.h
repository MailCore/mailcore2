//
//  MCIMAPRenameFolderOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPRenameFolderOperation__
#define __mailcore2__MCIMAPRenameFolderOperation__

#include <mailcore/MCIMAPOperation.h>

namespace mailcore {
    
    class IMAPRenameFolderOperation : public IMAPOperation {
    private:
        String * mFolder;
        String * mOtherName;
        
    public:
        IMAPRenameFolderOperation();
        ~IMAPRenameFolderOperation();
        
        virtual void setFolder(String * folder);
        virtual String * folder();
        
        virtual void setOtherName(String * otherName);
        virtual String * otherName();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__MCIMAPRenameFolderOperation__) */
