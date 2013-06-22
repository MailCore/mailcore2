//
//  MCPOPFetchMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCPOPFETCHMESSAGESOPERATION_H_

#define __MAILCORE_MCPOPFETCHMESSAGESOPERATION_H_

#include <MailCore/MCPOPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    class POPFetchMessagesOperation : public POPOperation {
    public:
        POPFetchMessagesOperation();
        virtual ~POPFetchMessagesOperation();
        
        virtual Array * /* POPMessageInfo */ messages();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        Array * /* POPMessageInfo */ mMessages;
    };
}

#endif

#endif
