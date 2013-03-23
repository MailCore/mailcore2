//
//  MCHTMLRendererCallback.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 2/2/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCHTMLRendererCallback__
#define __mailcore2__MCHTMLRendererCallback__

#include <mailcore/MCAbstract.h>
#include <mailcore/MCIMAP.h>

#ifdef __cplusplus

namespace mailcore {
    
    class MessageParser;
    
    class HTMLRendererIMAPCallback {
    public:
        virtual Data * dataForIMAPPart(String * folder, IMAPPart * part) { return NULL; }
        virtual void prefetchAttachmentIMAPPart(String * folder, IMAPPart * part) {}
        virtual void prefetchImageIMAPPart(String * folder, IMAPPart * part) {}
    };
    
    class HTMLRendererTemplateCallback {
    public:
        virtual bool canPreviewPart(AbstractPart * part);
        
        virtual HashMap * templateValuesForHeader(MessageHeader * header);
        virtual HashMap * templateValuesForPart(AbstractPart * part);
        
        virtual String * templateForMainHeader();
        virtual String * templateForImage();
        virtual String * templateForAttachment();
        virtual String * templateForMessage();
        virtual String * templateForEmbeddedMessage();
        virtual String * templateForEmbeddedMessageHeader();
        virtual String * templateForAttachmentSeparator();
        
        virtual String * filterHTMLForPart(String * html);
        virtual String * filterHTMLForMessage(String * html);
    };

}

#endif

#endif /* defined(__mailcore2__MCHTMLRendererCallback__) */
