//
//  MCIMAPCopyMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPCopyMessagesOperation__
#define __mailcore2__MCIMAPCopyMessagesOperation__

#include <mailcore/MCIMAPOperation.h>

namespace mailcore {
    
    class IMAPCopyMessagesOperation : public IMAPOperation {
    private:
        Array * mUids;
        String * mDestFolder;
        Array * mDestUids;
        
    public:
        IMAPCopyMessagesOperation();
        virtual ~IMAPCopyMessagesOperation();
        
        virtual void setDestFolder(String * destFolder);
        virtual String * destFolder();
        
        virtual void setUids(Array * uids);
        virtual Array * uids();
        
        virtual Array * destUids();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__MCIMAPCopyMessagesOperation__) */
