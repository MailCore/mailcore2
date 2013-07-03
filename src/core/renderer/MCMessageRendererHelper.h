//
//  MCMessageRendererHelper.h
//  mailcore2
//
//  Created by Paul Young on 28/06/2013.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCMESSAGERENDERERHELPER_H_

#define __MAILCORE_MCMESSAGERENDERERHELPER_H_

#include "MCHTMLRendererCallback.h"
#include "MCHTMLBodyRendererCallback.h"
#include "MCIMAPSession.h"

#ifdef __cplusplus

namespace mailcore {
    
    class MessageRendererHelper {
    public:
        MessageRendererHelper();
        ~MessageRendererHelper();

        virtual HTMLRendererIMAPCallback * dataCallback();
        virtual HTMLBodyRendererTemplateCallback * htmlBodyCallback();
    
    private:
        HTMLRendererIMAPCallback * mDataCallback;
        HTMLBodyRendererTemplateCallback * mHtmlCallback;
        
    };
}

#endif

#endif
