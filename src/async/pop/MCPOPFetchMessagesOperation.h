//
//  MCPOPFetchMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCPOPFetchMessagesOperation__
#define __mailcore2__MCPOPFetchMessagesOperation__

#include <mailcore/MCPOPOperation.h>

namespace mailcore {
    class POPFetchMessagesOperation : public POPOperation {
        Array * mMessages;
    public:
        POPFetchMessagesOperation();
        virtual ~POPFetchMessagesOperation();
        
        virtual Array * messages();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__MCPOPFetchMessagesOperation__) */
