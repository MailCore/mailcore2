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

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPIdleOperation : public IMAPOperation {
    public:
        IMAPIdleOperation();
        virtual ~IMAPIdleOperation();
        
        virtual void setLastKnownUID(uint32_t uid);
        virtual uint32_t lastKnownUID();
        
        virtual void cancel();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        uint32_t mLastKnownUid;
        void prepare();
        void unprepare();
        
    };
}

#endif

#endif /* defined(__mailcore2__IMAPIdleOperation__) */
