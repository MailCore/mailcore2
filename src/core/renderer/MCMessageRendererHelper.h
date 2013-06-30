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
#include "MCIMAPSession.h"

#ifdef __cplusplus

namespace mailcore {
    
    class MessageRendererHelper {
    public:
        virtual HTMLRendererIMAPCallback * dataCallback();
        virtual HTMLRendererTemplateCallback * htmlCallback();
        virtual HTMLBodyRendererTemplateCallback * htmlBodyCallback();
        
    }
}

#endif

#endif
