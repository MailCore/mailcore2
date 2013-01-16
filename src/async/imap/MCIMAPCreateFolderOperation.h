//
//  MCIMAPCreateFolderOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPCreateFolderOperation__
#define __mailcore2__MCIMAPCreateFolderOperation__

#include <mailcore/MCIMAPOperation.h>

namespace mailcore {
    
    class IMAPCreateFolderOperation : public IMAPOperation {
    private:
        String * mFolder;
        
    public:
        IMAPCreateFolderOperation();
        virtual ~IMAPCreateFolderOperation();
        
        virtual void setFolder(String * folder);
        virtual String * folder();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__MCIMAPCreateFolderOperation__) */
