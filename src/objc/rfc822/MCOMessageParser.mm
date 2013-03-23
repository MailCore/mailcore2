//
//  MCOMessageParser.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMessageParser.h"

#include "MCRFC822.h"
#include "MCRenderer.h"

#import "MCOHTMLRendererDelegate.h"
#import "NSObject+MCO.h"
#import "MCOAbstractMessage+Private.h"
#import "MCOUtils.h"

class MCOMessageParserRenderedCallback : public mailcore::Object, public mailcore::HTMLRendererTemplateCallback {
public:
    MCOMessageParserRenderedCallback(MCOMessageParser * parser)
    {
        _parser = parser;
    }
    
    virtual bool canPreviewPart(mailcore::AbstractPart * part)
    {
        if ([rendererDelegate() respondsToSelector:@selector(MCOAbstractMessage:canPreviewPart:)]) {
            return [rendererDelegate() MCOAbstractMessage:_parser canPreviewPart:MCO_TO_OBJC(part)];
        }
        return mailcore::HTMLRendererTemplateCallback::canPreviewPart(part);
    }
    
    virtual mailcore::HashMap * templateValuesForHeader(mailcore::MessageHeader * header)
    {
        if ([rendererDelegate() respondsToSelector:@selector(MCOAbstractMessage:templateValuesForHeader:)]) {
            return MCO_FROM_OBJC(mailcore::HashMap, [rendererDelegate() MCOAbstractMessage:_parser templateValuesForHeader:MCO_TO_OBJC(header)]);
        }
        return mailcore::HTMLRendererTemplateCallback::templateValuesForHeader(header);
    }
    
    virtual mailcore::HashMap * templateValuesForPart(mailcore::AbstractPart * part)
    {
        if ([rendererDelegate() respondsToSelector:@selector(MCOAbstractMessage:templateValuesForPart:)]) {
            return MCO_FROM_OBJC(mailcore::HashMap, [rendererDelegate() MCOAbstractMessage:_parser templateValuesForPart:MCO_TO_OBJC(part)]);
        }
        return mailcore::HTMLRendererTemplateCallback::templateValuesForPart(part);
    }
    
    virtual mailcore::String * templateForMainHeader()
    {
        if ([rendererDelegate() respondsToSelector:@selector(MCOAbstractMessage_templateForMainHeader:)]) {
            return MCO_FROM_OBJC(mailcore::String, [rendererDelegate() MCOAbstractMessage_templateForMainHeader:_parser]);
        }
        return mailcore::HTMLRendererTemplateCallback::templateForMainHeader();
    }
    
    virtual mailcore::String * templateForImage()
    {
        if ([rendererDelegate() respondsToSelector:@selector(MCOAbstractMessage_templateForImage:)]) {
            return MCO_FROM_OBJC(mailcore::String, [rendererDelegate() MCOAbstractMessage_templateForImage:_parser]);
        }
        return mailcore::HTMLRendererTemplateCallback::templateForImage();
    }
    
    virtual mailcore::String * templateForAttachment()
    {
        if ([rendererDelegate() respondsToSelector:@selector(MCOAbstractMessage_templateForAttachment:)]) {
            return MCO_FROM_OBJC(mailcore::String, [rendererDelegate() MCOAbstractMessage_templateForAttachment:_parser]);
        }
        return mailcore::HTMLRendererTemplateCallback::templateForAttachment();
    }
    
    virtual mailcore::String * templateForMessage()
    {
        if ([rendererDelegate() respondsToSelector:@selector(MCOAbstractMessage_templateForMessage:)]) {
            return MCO_FROM_OBJC(mailcore::String, [rendererDelegate() MCOAbstractMessage_templateForMessage:_parser]);
        }
        return mailcore::HTMLRendererTemplateCallback::templateForMessage();
    }
    
    virtual mailcore::String * templateForEmbeddedMessage()
    {
        if ([rendererDelegate() respondsToSelector:@selector(MCOAbstractMessage_templateForEmbeddedMessage:)]) {
            return MCO_FROM_OBJC(mailcore::String, [rendererDelegate() MCOAbstractMessage_templateForEmbeddedMessage:_parser]);
        }
        return mailcore::HTMLRendererTemplateCallback::templateForEmbeddedMessage();
    }
    
    virtual mailcore::String * templateForEmbeddedMessageHeader()
    {
        if ([rendererDelegate() respondsToSelector:@selector(MCOAbstractMessage_templateForEmbeddedMessageHeader:)]) {
            return MCO_FROM_OBJC(mailcore::String, [rendererDelegate() MCOAbstractMessage_templateForEmbeddedMessageHeader:_parser]);
        }
        return mailcore::HTMLRendererTemplateCallback::templateForEmbeddedMessageHeader();
    }
    
    virtual mailcore::String * templateForAttachmentSeparator()
    {
        if ([rendererDelegate() respondsToSelector:@selector(MCOAbstractMessage_templateForAttachmentSeparator:)]) {
            return MCO_FROM_OBJC(mailcore::String, [rendererDelegate() MCOAbstractMessage_templateForAttachmentSeparator:_parser]);
        }
        return mailcore::HTMLRendererTemplateCallback::templateForAttachmentSeparator();
    }
    
    virtual mailcore::String * filterHTMLForPart(mailcore::String * html)
    {
        if ([rendererDelegate() respondsToSelector:@selector(MCOAbstractMessage:filterHTMLForPart:)]) {
            return MCO_FROM_OBJC(mailcore::String, [rendererDelegate() MCOAbstractMessage:_parser filterHTMLForPart:MCO_TO_OBJC(html)]);
        }
        return mailcore::HTMLRendererTemplateCallback::filterHTMLForPart(html);
    }
    
    virtual mailcore::String * filterHTMLForMessage(mailcore::String * html)
    {
        if ([rendererDelegate() respondsToSelector:@selector(MCOAbstractMessage:filterHTMLForMesage:)]) {
            return MCO_FROM_OBJC(mailcore::String, [rendererDelegate() MCOAbstractMessage:_parser filterHTMLForMessage:MCO_TO_OBJC(html)]);
        }
        return mailcore::HTMLRendererTemplateCallback::filterHTMLForMessage(html);
    }
    
private:
    id <MCOHTMLRendererDelegate> rendererDelegate()
    {
        return [_parser rendererDelegate];
    }
    
    MCOMessageParser * _parser;
};

@implementation MCOMessageParser {
    id <MCOHTMLRendererDelegate> _rendererDelegate;
    MCOMessageParserRenderedCallback * _htmlRenderCallback;
}

@synthesize rendererDelegate = _rendererDelegate;

#define nativeType mailcore::MessageParser

+ (void) initialize
{
    MCORegisterClass(self, &typeid(nativeType));
}

+ (id) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::MessageParser * msg = (mailcore::MessageParser *) object;
    return [[[self alloc] initWithMCMessage:msg] autorelease];
}

+ (MCOMessageParser *) messageParserWithData:(NSData *)data
{
    return [[[MCOMessageParser alloc] initWithData:data] autorelease];
}

- (id) initWithData:(NSData *)data
{
    mailcore::MessageParser * message = new mailcore::MessageParser([data mco_mcData]);
    self = [super initWithMCMessage:message];
    _htmlRenderCallback = new MCOMessageParserRenderedCallback(self);
    message->release();
    return self;
}

- (void) dealloc
{
    _htmlRenderCallback->release();
    [super dealloc];
}

- (MCOAbstractPart *) mainPart
{
    return MCO_OBJC_BRIDGE_GET(mainPart);
}

- (NSData *) data
{
    return MCO_OBJC_BRIDGE_GET(data);
}

- (NSString *) htmlRendering
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->htmlRendering(_htmlRenderCallback));
}

@end
