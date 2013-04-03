//
//  MCPOPFetchMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCPOPFetchMessagesOperation__
#define __mailcore2__MCPOPFetchMessagesOperation__

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
        Array * mMessages;
    };
}

#endif

#endif /* defined(__mailcore2__MCPOPFetchMessagesOperation__) */
