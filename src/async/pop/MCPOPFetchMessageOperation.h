//
//  MCPOPFetchMessageOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCPOPFetchMessageOperation__
#define __mailcore2__MCPOPFetchMessageOperation__

#include <mailcore/MCPOPOperation.h>

namespace mailcore {
    class POPFetchMessageOperation : public POPOperation {
    private:
        unsigned int mMessageIndex;
        Data * mData;
        
    public:
        POPFetchMessageOperation();
        virtual ~POPFetchMessageOperation();
        
        virtual void setMessageIndex(unsigned int messageIndex);
        virtual unsigned int messageIndex();
        
        virtual Data * data();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__MCPOPFetchMessageOperation__) */
