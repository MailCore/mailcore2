//
//  MCPOPDeleteMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCPOPDeleteMessagesOperation__
#define __mailcore2__MCPOPDeleteMessagesOperation__

#include <mailcore/MCPOPOperation.h>

namespace mailcore {
    
    class POPDeleteMessagesOperation : public POPOperation {
    private:
        Array * mMessageIndexes;
        
    public:
        POPDeleteMessagesOperation();
        virtual ~POPDeleteMessagesOperation();
        
        virtual void setMessageIndexes(Array * indexes);
        virtual Array * messageIndexes();
        
        virtual void main();
    };
    
}


#endif /* defined(__mailcore2__MCPOPDeleteMessagesOperation__) */
