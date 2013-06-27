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
    
    typedef enum {
        RenderingTypeHTML,
        RenderingTypeHTMLBody,
        RenderingTypePlainText,
        RenderingTypePlainTextBody
    } RenderingType;
    
    class IMAPMessageRenderingOperation : public IMAPOperation {
    public:
        IMAPMessageRenderingOperation();
        virtual ~IMAPMessageRenderingOperation();
        
        virtual void setRenderingType(RenderingType type);
        virtual RenderingType renderingType();
        
        virtual void setUid(uint32_t uid);
        virtual uint32_t uid();
        
        // Result.
        virtual String * result();
        
    public: // subclass behavior
        virtual void main();
    
    private:
        RenderingType mRenderingType;
        String * mResult;
        uint32_t mUid;
        
    };
}

#endif

#endif
