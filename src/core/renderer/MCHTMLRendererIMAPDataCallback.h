//
//  MCHTMLRendererIMAPDataCallback.h
//  mailcore2
//
//  Created by Paul Young on 06/07/2013.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCHTMLRENDERERIMAPDATACALLBACK_H_

#define __MAILCORE_MCHTMLRENDERERIMAPDATACALLBACK_H_

#include "MCHTMLRendererCallback.h"

#ifdef __cplusplus

namespace mailcore {
    
    class HTMLRendererIMAPDataCallback : public Object, public HTMLRendererIMAPCallback {
    public:
        HTMLRendererIMAPDataCallback();
        
        virtual Data * dataForIMAPPart(String * folder, IMAPPart * part);
        virtual ErrorCode error();
    
    private:
        IMAPSession * mSession;
        uint32_t mUid;
        ErrorCode mError;
    };
    
}

#endif

#endif
