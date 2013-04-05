//
//  MCIMAPCopyMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPCOPYMESSAGESOPERATION_H_

#define __MAILCORE_MCIMAPCOPYMESSAGESOPERATION_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPCopyMessagesOperation : public IMAPOperation {
    public:
        IMAPCopyMessagesOperation();
        virtual ~IMAPCopyMessagesOperation();
        
        virtual void setDestFolder(String * destFolder);
        virtual String * destFolder();
        
        virtual void setUids(IndexSet * uids);
        virtual IndexSet * uids();
        
        // Result.
        virtual IndexSet * destUids();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        IndexSet * mUids;
        String * mDestFolder;
        IndexSet * mDestUids;
    };
}

#endif

#endif
