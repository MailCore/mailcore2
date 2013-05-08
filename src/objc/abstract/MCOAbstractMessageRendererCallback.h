//
//  MCOAbstractMessageRendererCallback.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOABSTRACTMESSAGERENDERECALLBACK_H_

#define __MAILCORE_MCOABSTRACTMESSAGERENDERECALLBACK_H_

#include "MCIMAP.h"
#include "MCRenderer.h"

#if defined(__cplusplus) && defined(__OBJC__)
@class MCOAbstractMessage;
@protocol MCOHTMLRendererDelegate;
@protocol MCOHTMLRendererIMAPDelegate;

class MCOAbstractMessageRendererCallback : public mailcore::Object, public mailcore::HTMLRendererTemplateCallback, public mailcore::HTMLRendererIMAPCallback {
public:
    MCOAbstractMessageRendererCallback(MCOAbstractMessage * message, id <MCOHTMLRendererDelegate> rendererDelegate,
                                     id <MCOHTMLRendererIMAPDelegate> rendererIMAPDelegate);
    virtual bool canPreviewPart(mailcore::AbstractPart * part);
    virtual bool shouldShowPart(mailcore::AbstractPart * part);
    virtual mailcore::HashMap * templateValuesForHeader(mailcore::MessageHeader * header);
    virtual mailcore::HashMap * templateValuesForPart(mailcore::AbstractPart * part);
    virtual mailcore::String * templateForMainHeader();
    virtual mailcore::String * templateForImage();
    virtual mailcore::String * templateForAttachment();
    virtual mailcore::String * templateForMessage();
    virtual mailcore::String * templateForEmbeddedMessage();
    virtual mailcore::String * templateForEmbeddedMessageHeader();
    virtual mailcore::String * templateForAttachmentSeparator();
    virtual mailcore::String * filterHTMLForPart(mailcore::String * html);
    virtual mailcore::String * filterHTMLForMessage(mailcore::String * html);
    
    virtual mailcore::Data * dataForIMAPPart(mailcore::String * folder, mailcore::IMAPPart * part);
    virtual void prefetchAttachmentIMAPPart(mailcore::String * folder, mailcore::IMAPPart * part);
    virtual void prefetchImageIMAPPart(mailcore::String * folder, mailcore::IMAPPart * part);

private:
    MCOAbstractMessage * mMessage;
    id <MCOHTMLRendererDelegate> mRendererDelegate;
    id <MCOHTMLRendererIMAPDelegate> mIMAPDelegate;
};
#endif

#endif
