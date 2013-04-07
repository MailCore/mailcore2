//
//  MCPOPCheckAccountOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 4/6/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCPOPCHECKACCOUNTOPERATION_H_
#define __MAILCORE_MCPOPCHECKACCOUNTOPERATION_H_

#include <MailCore/MCPOPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class POPCheckAccountOperation : public POPOperation {
    public:
        POPCheckAccountOperation();
        virtual ~POPCheckAccountOperation();
        
    public: // subclass behavior
        virtual void main();
    };
    
}

#endif

#endif
