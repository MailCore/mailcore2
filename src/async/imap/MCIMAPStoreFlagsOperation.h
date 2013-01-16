//
//  MCIMAPStoreFlagsOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPStoreFlagsOperation__
#define __mailcore2__MCIMAPStoreFlagsOperation__

#include <mailcore/MCIMAPOperation.h>

namespace mailcore {
    
    class IMAPStoreFlagsOperation : public IMAPOperation {
    private:
        String * mFolder;
        Array * mUids;
        IMAPStoreFlagsRequestKind mKind;
        MessageFlag mFlags;
        
    public:
        IMAPStoreFlagsOperation();
        virtual ~IMAPStoreFlagsOperation();
        
        virtual void setFolder(String * folder);
        virtual String * folder();
        
        virtual void setUids(Array * uids);
        virtual Array * uids();
        
        virtual void setKind(IMAPStoreFlagsRequestKind kind);
        virtual IMAPStoreFlagsRequestKind kind();
        
        virtual void setFlags(MessageFlag flags);
        virtual MessageFlag flags();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__MCIMAPStoreFlagsOperation__) */
