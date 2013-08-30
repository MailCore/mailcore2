//
//  IMAPIdentityOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPIDENTITYOPERATION_H_

#define __MAILCORE_MCIMAPIDENTITYOPERATION_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPIdentity;
    
    class IMAPIdentityOperation : public IMAPOperation {
    public:
        IMAPIdentityOperation();
        virtual ~IMAPIdentityOperation();
        
        virtual void setClientIdentity(IMAPIdentity * identity);
        virtual IMAPIdentity * clientIdentity();
        
        // Result.
        virtual IMAPIdentity * serverIdentity();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        IMAPIdentity * mClientIdentity;
        IMAPIdentity * mServerIdentity;
        
    };
}

#endif

#endif
