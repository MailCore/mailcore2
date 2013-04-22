//
//  MCHTMLRenderer.cpp
//  testUI
//
//  Created by DINH Viêt Hoà on 1/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCHTMLRenderer.h"

#include <ctemplate/template.h>
#include "MCAddressDisplay.h"
#include "MCDateFormatter.h"
#include "MCSizeFormatter.h"
#include "MCHTMLRendererCallback.h"

using namespace mailcore;

enum {
    RENDER_STATE_NONE,
    RENDER_STATE_HAD_ATTACHMENT,
    RENDER_STATE_HAD_ATTACHMENT_THEN_TEXT,
};

struct htmlRendererContext {
    HTMLRendererIMAPCallback * dataCallback;
    HTMLRendererTemplateCallback * htmlCallback;
    int firstRendered;
    String * folder;
    int state;
    int pass;
    bool hasMixedTextAndAttachments;
    bool firstAttachment;
    bool hasTextPart;
};

class DefaultTemplateCallback : public Object, public HTMLRendererTemplateCallback {
};

static bool partContainsMimeType(AbstractPart * part, String * mimeType);
static bool singlePartContainsMimeType(AbstractPart * part, String * mimeType);
static bool multipartContainsMimeType(AbstractMultipart * part, String * mimeType);
static bool messagePartContainsMimeType(AbstractMessagePart * part, String * mimeType);

static String * htmlForAbstractPart(AbstractPart * part, htmlRendererContext * context);

static String * renderTemplate(String * templateContent, HashMap * values);

static String * htmlForAbstractMessage(String * folder, AbstractMessage * message,
                                       HTMLRendererIMAPCallback * dataCallback,
                                       HTMLRendererTemplateCallback * htmlCallback);

static bool isTextPart(AbstractPart * part, htmlRendererContext * context)
{
    String * mimeType = part->mimeType()->lowercaseString();
    MCAssert(mimeType != NULL);
    
    if (!part->isInlineAttachment()) {
        if ((part->filename() != NULL) && context->firstRendered) {
            return false;
        }
    }
    
    if (mimeType->isEqual(MCSTR("text/plain"))) {
        return true;
    }
    else if (mimeType->isEqual(MCSTR("text/html"))) {
        return true;
    }
    else {
        return false;
    }
}


static AbstractPart * preferredPartInMultipartAlternative(AbstractMultipart * part)
{
    int htmlPart = -1;
    int textPart = -1;
    
    for(unsigned int i = 0 ; i < part->parts()->count() ; i ++) {
        AbstractPart * subpart = (AbstractPart *) part->parts()->objectAtIndex(i);
        if (partContainsMimeType(subpart, MCSTR("text/html"))) {
            htmlPart = i;
        }
        else if (partContainsMimeType(subpart, MCSTR("text/plain"))) {
            textPart = i;
        }
    }
    if (htmlPart != -1) {
        return (AbstractPart *) part->parts()->objectAtIndex(htmlPart);
    }
    else if (textPart != -1) {
        return (AbstractPart *) part->parts()->objectAtIndex(textPart);
    }
    else if (part->parts()->count() > 0) {
        return (AbstractPart *) part->parts()->objectAtIndex(0);
    }
    else {
        return NULL;
    }
}

static bool partContainsMimeType(AbstractPart * part, String * mimeType)
{
    switch (part->partType()) {
        case PartTypeSingle:
            return singlePartContainsMimeType(part, mimeType);
        case PartTypeMessage:
            return messagePartContainsMimeType((AbstractMessagePart *) part, mimeType);
        case PartTypeMultipartMixed:
        case PartTypeMultipartRelated:
        case PartTypeMultipartAlternative:
            return multipartContainsMimeType((AbstractMultipart *) part, mimeType);
        default:
            return false;
    }
}

static bool singlePartContainsMimeType(AbstractPart * part, String * mimeType)
{
    return part->mimeType()->lowercaseString()->isEqual(mimeType);
}

static bool multipartContainsMimeType(AbstractMultipart * part, String * mimeType)
{
    for(unsigned int i = 0 ; i < part->parts()->count() ; i ++) {
        AbstractPart * subpart = (AbstractPart *) part->parts()->objectAtIndex(i);
        if (partContainsMimeType(subpart, mimeType)) {
            return true;
        }
    }
    return false;
}

static bool messagePartContainsMimeType(AbstractMessagePart * part, String * mimeType)
{
    return partContainsMimeType(part->mainPart(), mimeType);
}

static String * htmlForAbstractMessage(String * folder, AbstractMessage * message,
                                       HTMLRendererIMAPCallback * dataCallback,
                                       HTMLRendererTemplateCallback * htmlCallback)
{
    AbstractPart * mainPart = NULL;
    
    if (htmlCallback == NULL) {
        htmlCallback = new DefaultTemplateCallback();
        ((DefaultTemplateCallback *) htmlCallback)->autorelease();
    }
    
    if (message->className()->isEqual(MCSTR("mailcore::IMAPMessage"))) {
        mainPart = ((IMAPMessage *) message)->mainPart();
    }
    else if (message->className()->isEqual(MCSTR("mailcore::MessageParser"))) {
        mainPart = ((MessageParser *) message)->mainPart();
    }
    MCAssert(mainPart != NULL);
    
    htmlRendererContext context;
    context.dataCallback = dataCallback;
    context.htmlCallback = htmlCallback;
    context.firstRendered = 0;
    context.folder = folder;
    context.state = RENDER_STATE_NONE;
    
    context.hasMixedTextAndAttachments = false;
    context.pass = 0;
    context.firstAttachment = false;
    context.hasTextPart = false;
    htmlForAbstractPart(mainPart, &context);
    
    context.hasMixedTextAndAttachments = (context.state == RENDER_STATE_HAD_ATTACHMENT_THEN_TEXT);
    context.pass = 1;
    context.firstAttachment = false;
    context.hasTextPart = false;
    String * content = htmlForAbstractPart(mainPart, &context);
    if (content == NULL)
        return NULL;
    
    content = htmlCallback->filterHTMLForMessage(content);
    
    HashMap * values = htmlCallback->templateValuesForHeader(message->header());
    String * headerString = renderTemplate(htmlCallback->templateForMainHeader(), values);
    
    HashMap * msgValues = new HashMap();
    msgValues->setObjectForKey(MCSTR("HEADER"), headerString);
    msgValues->setObjectForKey(MCSTR("BODY"), content);
    String * result = renderTemplate(htmlCallback->templateForMessage(), msgValues);
    msgValues->release();
    
    return result;
}

String * htmlForAbstractSinglePart(AbstractPart * part, htmlRendererContext * context);
String * htmlForAbstractMessagePart(AbstractMessagePart * part, htmlRendererContext * context);
String * htmlForAbstractMultipartRelated(AbstractMultipart * part, htmlRendererContext * context);
String * htmlForAbstractMultipartMixed(AbstractMultipart * part, htmlRendererContext * context);
String * htmlForAbstractMultipartAlternative(AbstractMultipart * part, htmlRendererContext * context);

String * htmlForAbstractPart(AbstractPart * part, htmlRendererContext * context)
{
    switch (part->partType()) {
        case PartTypeSingle:
            return htmlForAbstractSinglePart((AbstractPart *) part, context);
        case PartTypeMessage:
            return htmlForAbstractMessagePart((AbstractMessagePart *) part, context);
        case PartTypeMultipartMixed:
            return htmlForAbstractMultipartMixed((AbstractMultipart *) part, context);
        case PartTypeMultipartRelated:
            return htmlForAbstractMultipartRelated((AbstractMultipart *) part, context);
        case PartTypeMultipartAlternative:
            return htmlForAbstractMultipartAlternative((AbstractMultipart *) part, context);
        default:
            MCAssert(0);
    }
    return NULL;
}

String * htmlForAbstractSinglePart(AbstractPart * part, htmlRendererContext * context)
{
    String * mimeType = NULL;
    if (part->mimeType() != NULL) {
        mimeType = part->mimeType()->lowercaseString();
    }
    MCAssert(mimeType != NULL);
    
    if (isTextPart(part, context)) {
        if (context->pass == 0) {
            if (context->state == RENDER_STATE_HAD_ATTACHMENT) {
                context->state = RENDER_STATE_HAD_ATTACHMENT_THEN_TEXT;
            }
            return NULL;
        }
        
        context->hasTextPart = true;
        
        if (mimeType->isEqual(MCSTR("text/plain"))) {
            String * charset = part->charset();
            Data * data = NULL;
            if (part->className()->isEqual(MCSTR("mailcore::IMAPPart"))) {
                data = context->dataCallback->dataForIMAPPart(context->folder, (IMAPPart *) part);
            }
            else if (part->className()->isEqual(MCSTR("mailcore::Attachment"))) {
                data = ((Attachment *) part)->data();
                MCAssert(data != NULL);
            }
            if (data == NULL)
                return NULL;
            
            String * str = data->stringWithDetectedCharset(charset, false);
            context->firstRendered = true;
            return str->htmlEncodedString();
        }
        else if (mimeType->isEqual(MCSTR("text/html"))) {
            String * charset = part->charset();
            Data * data = NULL;
            if (part->className()->isEqual(MCSTR("mailcore::IMAPPart"))) {
                data = context->dataCallback->dataForIMAPPart(context->folder, (IMAPPart *) part);
            }
            else if (part->className()->isEqual(MCSTR("mailcore::Attachment"))) {
                data = ((Attachment *) part)->data();
                MCAssert(data != NULL);
            }
            if (data == NULL)
                return NULL;
            
            String * str = data->stringWithDetectedCharset(charset, true);
            str = str->cleanedHTMLString();
            str = context->htmlCallback->filterHTMLForPart(str);
            context->firstRendered = true;
            return str;
        }
        else {
            MCAssert(0);
            return NULL;
        }
    }
    else {
        if (context->pass == 0) {
            if (context->state == RENDER_STATE_NONE) {
                context->state = RENDER_STATE_HAD_ATTACHMENT;
            }
            return NULL;
        }
        
        if (part->uniqueID() == NULL) {
            part->setUniqueID(String::uuidString());
        }
        
        String * result = String::string();
        String * separatorString;
        String * content;
        
        if (!context->firstAttachment && context->hasTextPart) {
            separatorString = context->htmlCallback->templateForAttachmentSeparator();
        }
        else {
            separatorString = MCSTR("");
        }
        
        context->firstAttachment = true;
        
        if (context->htmlCallback->canPreviewPart(part)) {
            if (part->className()->isEqual(MCSTR("mailcore::IMAPPart"))) {
                context->dataCallback->prefetchImageIMAPPart(context->folder, (IMAPPart *) part);
            }
            String * url = String::stringWithUTF8Format("x-mailcore-image:%s",
                                                                            part->uniqueID()->UTF8Characters());
            HashMap * values = context->htmlCallback->templateValuesForPart(part);
            values->setObjectForKey(MCSTR("URL"), url);
            content = renderTemplate(context->htmlCallback->templateForImage(), values);
        }
        else {
            if (part->className()->isEqual(MCSTR("mailcore::IMAPPart"))) {
                context->dataCallback->prefetchAttachmentIMAPPart(context->folder, (IMAPPart *) part);
            }
            HashMap * values = context->htmlCallback->templateValuesForPart(part);
            content = renderTemplate(context->htmlCallback->templateForAttachment(), values);
        }
        
        result->appendString(separatorString);
        result->appendString(content);
        
        return result;
    }
}

String * htmlForAbstractMessagePart(AbstractMessagePart * part, htmlRendererContext * context)
{
    if (context->pass == 0) {
        return NULL;
    }
    String * substring = htmlForAbstractPart(part->mainPart(), context);
    MCAssert(substring != NULL);
    
    HashMap * values = context->htmlCallback->templateValuesForHeader(part->header());
    String * headerString = renderTemplate(context->htmlCallback->templateForEmbeddedMessageHeader(), values);
	
	HashMap * msgValues = new HashMap();
    msgValues->setObjectForKey(MCSTR("HEADER"), headerString);
    msgValues->setObjectForKey(MCSTR("BODY"), substring);
    String * result = renderTemplate(context->htmlCallback->templateForEmbeddedMessage(), msgValues);
    msgValues->release();

    return result;
}

String * htmlForAbstractMultipartAlternative(AbstractMultipart * part, htmlRendererContext * context)
{
    AbstractPart * preferredAlternative = preferredPartInMultipartAlternative(part);
    if (preferredAlternative == NULL)
        return MCSTR("");
    
    return htmlForAbstractPart(preferredAlternative, context);
}

String * htmlForAbstractMultipartMixed(AbstractMultipart * part, htmlRendererContext * context)
{
    String * result = String::string();
    for(unsigned int i = 0 ; i < part->parts()->count() ; i ++) {
        AbstractPart * subpart = (AbstractPart *) part->parts()->objectAtIndex(i);
        String * substring = htmlForAbstractPart(subpart, context);
        if (context->pass != 0) {
            if (substring == NULL)
                return NULL;
            
            result->appendString(substring);
        }
    }
    return  result;
}

String * htmlForAbstractMultipartRelated(AbstractMultipart * part, htmlRendererContext * context)
{
    if (part->parts()->count() == 0) {
        if (context->pass == 0) {
            return NULL;
        }
        else {
            return MCSTR("");
        }
    }
    
    AbstractPart * subpart = (AbstractPart *) part->parts()->objectAtIndex(0);
    return htmlForAbstractPart(subpart, context);
}

void fillTemplateDictionaryFromMCHashMap(ctemplate::TemplateDictionary * dict, HashMap * mcHashMap)
{
    Array * keys = mcHashMap->allKeys();
    
    for(unsigned int i = 0 ; i < keys->count() ; i ++) {
        String * key = (String *) keys->objectAtIndex(i);
        Object * value;
        
        value = mcHashMap->objectForKey(key);
        if (value->className()->isEqual(MCSTR("mailcore::String"))) {
            String * str;
            
            str = (String *) value;
            dict->SetValue(key->UTF8Characters(), str->UTF8Characters());
        }
        else if (value->className()->isEqual(MCSTR("mailcore::Array"))) {
            Array * array;
            
            array = (Array *) value;
            for(unsigned int k = 0 ; k < array->count() ; k ++) {
                HashMap * item = (HashMap *) array->objectAtIndex(k);
                ctemplate::TemplateDictionary * subDict = dict->AddSectionDictionary(key->UTF8Characters());
                fillTemplateDictionaryFromMCHashMap(subDict, item);
            }
        }
        else if (value->className()->isEqual(MCSTR("mailcore::HashMap"))) {
            ctemplate::TemplateDictionary * subDict;
            HashMap * item;
            
            item = (HashMap *) value;
            subDict = dict->AddSectionDictionary(key->UTF8Characters());
            fillTemplateDictionaryFromMCHashMap(subDict, item);
        }
    }
}

String * renderTemplate(String * templateContent, HashMap * values)
{
    ctemplate::TemplateDictionary dict("template dict");
    std::string output;
    Data * data;
    
    fillTemplateDictionaryFromMCHashMap(&dict, values);
    data = templateContent->dataUsingEncoding("utf-8");
    ctemplate::Template * tpl = ctemplate::Template::StringToTemplate(data->bytes(), data->length(), ctemplate::DO_NOT_STRIP);
    if (tpl == NULL)
        return NULL;
    if (!tpl->Expand(&output, &dict))
        return NULL;
    delete tpl;
    
    return String::stringWithUTF8Characters(output.c_str());
}

String * HTMLRenderer::htmlForRFC822Message(MessageParser * message,
                                            HTMLRendererTemplateCallback * htmlCallback)
{
    return htmlForAbstractMessage(NULL, message, NULL, htmlCallback);
}

String * HTMLRenderer::htmlForIMAPMessage(String * folder,
                                          IMAPMessage * message,
                                          HTMLRendererIMAPCallback * dataCallback,
                                          HTMLRendererTemplateCallback * htmlCallback)
{
    return htmlForAbstractMessage(folder, message, dataCallback, htmlCallback);
}
