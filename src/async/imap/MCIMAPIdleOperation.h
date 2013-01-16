//
//  IMAPIdleOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__IMAPIdleOperation__
#define __mailcore2__IMAPIdleOperation__

#include <mailcore/MCIMAPOperation.h>

namespace mailcore {
    
    class IMAPIdleOperation : public IMAPOperation {
    private:
        String * mFolder;
        uint32_t mLastKnownUid;
        void prepare();
        void unprepare();
        
    public:
        IMAPIdleOperation();
        virtual ~IMAPIdleOperation();
        
        virtual void setFolder(String * folder);
        virtual String * folder();
        
        virtual void setLastKnownUID(uint32_t uid);
        virtual uint32_t lastKnownUID();
        
        virtual void cancel();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__IMAPIdleOperation__) */
