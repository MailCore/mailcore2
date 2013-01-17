//
//  MCPOPFetchHeaderOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCPOPFetchHeaderOperation__
#define __mailcore2__MCPOPFetchHeaderOperation__

#include <mailcore/MCPOPOperation.h>

namespace mailcore {
    
    class MessageHeader;
    
    class POPFetchHeaderOperation : public POPOperation {
    private:
        unsigned int mMessageIndex;
        MessageHeader * mHeader;
        
    public:
        POPFetchHeaderOperation();
        virtual ~POPFetchHeaderOperation();
        
        virtual void setMessageIndex(unsigned int messageIndex);
        virtual unsigned int messageIndex();
        
        virtual MessageHeader * header();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__MCPOPFetchHeaderOperation__) */
