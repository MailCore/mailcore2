//
//  MCIMAPStoreFlagsOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPSTOREFLAGSOPERATION_H_

#define __MAILCORE_MCIMAPSTOREFLAGSOPERATION_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPStoreFlagsOperation : public IMAPOperation {
    public:
        IMAPStoreFlagsOperation();
        virtual ~IMAPStoreFlagsOperation();
        
        virtual void setUids(IndexSet * uids);
        virtual IndexSet * uids();
        
        virtual void setKind(IMAPStoreFlagsRequestKind kind);
        virtual IMAPStoreFlagsRequestKind kind();
        
        virtual void setFlags(MessageFlag flags);
        virtual MessageFlag flags();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        IndexSet * mUids;
        IMAPStoreFlagsRequestKind mKind;
        MessageFlag mFlags;
        
    };
    
}

#endif

#endif
