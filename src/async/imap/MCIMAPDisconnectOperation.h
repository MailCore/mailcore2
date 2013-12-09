//
//  MCIMAPDisconnectOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 6/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPDISCONNECTOPERATION_H_
#define __MAILCORE_MCIMAPDISCONNECTOPERATION_H_

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCAbstract.h>
#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPDisconnectOperation : public IMAPOperation {
    public:
        IMAPDisconnectOperation();
        virtual ~IMAPDisconnectOperation();
        
    public: // subclass behavior
        virtual void main();
    };
    
}

#endif

#endif