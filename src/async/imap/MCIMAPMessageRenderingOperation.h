//
//  MCIMAPMessageRenderingOperation.h
//  mailcore2
//
//  Created by Paul Young on 27/06/2013.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPMessageRenderingOperation_H_

#define __MAILCORE_MCIMAPMessageRenderingOperation_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPMessageRenderingOperation : public IMAPOperation {
    public:
        IMAPMessageRenderingOperation();
        virtual ~IMAPMessageRenderingOperation();
        
        virtual void setUid(uint32_t uid);
        virtual uint32_t uid();
        
        // Results.        
        virtual void htmlRendering();
        virtual void htmlBodyRendering();
        
        virtual void plainTextRendering();
        virtual void plainTextBodyRendering();
        
    public: // subclass behavior
        virtual void main();
    
    private:
        uint32_t mUid;
        Data * mData;
        
    };
}

#endif

#endif
