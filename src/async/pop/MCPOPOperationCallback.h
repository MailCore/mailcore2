//
//  MCPOPOperationCallback.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCPOPOPERATIONCALLBACK_H_

#define __MAILCORE_MCPOPOPERATIONCALLBACK_H_

#ifdef __cplusplus

namespace mailcore {
    
    class POPOperation;
    
    class POPOperationCallback {
    public:
        virtual void bodyProgress(POPOperation * session, unsigned int current, unsigned int maximum) {};
    };
    
}

#endif

#endif
