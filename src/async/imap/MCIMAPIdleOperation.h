//
//  IMAPIdleOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPIDLEOPERATION_H_

#define __MAILCORE_MCIMAPIDLEOPERATION_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPIdleOperation : public IMAPOperation {
    public:
        IMAPIdleOperation();
        virtual ~IMAPIdleOperation();
        
        virtual void setLastKnownUID(uint32_t uid);
        virtual uint32_t lastKnownUID();
        
        virtual void interruptIdle();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        uint32_t mLastKnownUid;
        bool mSetupSuccess;
        void prepare();
        void unprepare();
    };
}

#endif

#endif
