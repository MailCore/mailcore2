//
//  MCIMAPConnectOperation.h
//  mailcore2
//
//  Created by Ryan Walklin on 6/09/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPCONNECTOPERATION_H_
#define __MAILCORE_MCIMAPCONNECTOPERATION_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPConnectOperation : public IMAPOperation {
    public: // subclass behavior
        virtual void main();
    };
    
}

#endif

#endif /* defined(__MAILCORE_MCIMAPCONNECTOPERATION_H_) */
