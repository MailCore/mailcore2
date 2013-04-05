//
//  MCPOPDeleteMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCPOPDELETEMESSAGESOPERATION_H_

#define __MAILCORE_MCPOPDELETEMESSAGESOPERATION_H_

#include <MailCore/MCPOPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class POPDeleteMessagesOperation : public POPOperation {
    public:
        POPDeleteMessagesOperation();
        virtual ~POPDeleteMessagesOperation();
        
        virtual void setMessageIndexes(IndexSet * indexes);
        virtual IndexSet * messageIndexes();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        IndexSet * mMessageIndexes;
        
    };
    
}

#endif

#endif
