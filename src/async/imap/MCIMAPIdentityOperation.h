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
    
    class IMAPIdentityOperation : public IMAPOperation {
    public:
        IMAPIdentityOperation();
        virtual ~IMAPIdentityOperation();
        
        virtual void setVendor(String * vendor);
        virtual String * vendor();
        
        virtual void setName(String * name);
        virtual String * name();
        
        virtual void setVersion(String * version);
        virtual String * version();
        
        // Result.
        virtual HashMap * serverIdentity();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        String * mVendor;
        String * mName;
        String * mVersion;
        HashMap * mServerIdentity;
        
    };
}

#endif

#endif
