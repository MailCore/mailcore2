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

#ifdef __cplusplus

namespace mailcore {
    
    class MessageHeader;
    
    class POPFetchHeaderOperation : public POPOperation {
    public:
        POPFetchHeaderOperation();
        virtual ~POPFetchHeaderOperation();
        
        virtual void setMessageIndex(unsigned int messageIndex);
        virtual unsigned int messageIndex();
        
        virtual MessageHeader * header();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        unsigned int mMessageIndex;
        MessageHeader * mHeader;
        
    };
}

#endif

#endif /* defined(__mailcore2__MCPOPFetchHeaderOperation__) */
