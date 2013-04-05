//
//  MCPOPFetchMessageOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCPOPFETCHMESSAGEOPERATION_H_

#define __MAILCORE_MCPOPFETCHMESSAGEOPERATION_H_

#include <MailCore/MCPOPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    class POPFetchMessageOperation : public POPOperation {
    public:
        POPFetchMessageOperation();
        virtual ~POPFetchMessageOperation();
        
        virtual void setMessageIndex(unsigned int messageIndex);
        virtual unsigned int messageIndex();
        
        virtual Data * data();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        unsigned int mMessageIndex;
        Data * mData;
        
    };
}

#endif

#endif
