//
//  MCHTMLBodyRendererTemplateCallback.h
//  mailcore2
//
//  Created by Micah Rosales on 08/01/2013.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCHTMLBARERENDERERTEMPLATECALLBACK_H_

#define __MAILCORE_MCHTMLBARERENDERERTEMPLATECALLBACK_H_

#include <MailCore/MCHTMLRendererCallback.h>

#ifdef __cplusplus

namespace mailcore {
	
    class HTMLBareRendererTemplateCallback : public Object, public HTMLRendererTemplateCallback {
    public:
        virtual String * templateForMainHeader(MessageHeader * header);
		virtual String * templateForAttachment(AbstractPart * part);
        virtual String * templateForMessage(AbstractMessage * message);


        // Can be used to filter some HTML tags.
        virtual String * filterHTMLForPart(String * html);
        
        // Can be used to hide quoted text.
        virtual String * filterHTMLForMessage(String * html);
    };
    
}

#endif

#endif
