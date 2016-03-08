//
//  MCIMAPCheckAccountOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCIMAPCHECKACCOUNTOPERATION_H

#define MAILCORE_MCIMAPCHECKACCOUNTOPERATION_H

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class MAILCORE_EXPORT IMAPCheckAccountOperation : public IMAPOperation {
    public:
        IMAPCheckAccountOperation();
        virtual ~IMAPCheckAccountOperation();

        virtual String * loginResponse();

    public: // subclass behavior
        virtual void main();

    private:
        String * mLoginResponse;
    };
    
}

#endif

#endif
