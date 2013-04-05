//
//  MCIMAPOperationCallback.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPOPERATIONCALLBACK_H_

#define __MAILCORE_MCIMAPOPERATIONCALLBACK_H_

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPOperation;
    
    class IMAPOperationCallback {
    public:
        virtual void bodyProgress(IMAPOperation * session, unsigned int current, unsigned int maximum) {};
        virtual void itemProgress(IMAPOperation * session, unsigned int current, unsigned int maximum) {};
    };
    
}

#endif

#endif
