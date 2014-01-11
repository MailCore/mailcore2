//
//  MCHTMLRenderer.h
//  testUI
//
//  Created by DINH Viêt Hoà on 1/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCHTMLRENDERER_H_
#define __MAILCORE_MCHTMLRENDERER_H_

#include <MailCore/MCAbstract.h>
#include <MailCore/MCIMAP.h>
#include <MailCore/MCRFC822.h>

#ifdef __cplusplus

namespace mailcore {
    
    class MessageParser;
    class HTMLRendererTemplateCallback;
    class HTMLRendererIMAPCallback;
    
    class HTMLRenderer {
    public:
        static String * htmlForRFC822Message(MessageParser * message,
                                             HTMLRendererTemplateCallback * htmlCallback);
        
        static String * htmlForIMAPMessage(String * folder,
                                           IMAPMessage * message,
                                           HTMLRendererIMAPCallback * dataCallback,
                                           HTMLRendererTemplateCallback * htmlCallback);
        
        static Array * /* AbstractPart */ attachmentsForMessage(AbstractMessage * message);
        static Array * /* AbstractPart */ htmlInlineAttachmentsForMessage(AbstractMessage * message);
    };
    
};

#endif

#endif
