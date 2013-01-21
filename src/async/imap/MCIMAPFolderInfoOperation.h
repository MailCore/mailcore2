//
//  MCIMAPFolderInfoOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/13/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPFolderInfoOperation__
#define __mailcore2__MCIMAPFolderInfoOperation__

#include <mailcore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPFolderInfoOperation : public IMAPOperation {
    public:
        IMAPFolderInfoOperation();
        virtual ~IMAPFolderInfoOperation();
        
        virtual uint32_t uidNext();
        virtual uint32_t uidValidity();
        virtual int messageCount();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        uint32_t mUidNext;
        uint32_t mUidValidity;
        int mMessageCount;
        
    };

}

#endif

#endif /* defined(__mailcore2__MCIMAPFolderInfoOperation__) */
