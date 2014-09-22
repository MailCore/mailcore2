//
//  MCOAttachment.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOAttachment.h"

#include "MCAttachment.h"

#import "MCOAbstractPart+Private.h"
#import "MCOUtils.h"
#import "NSString+MCO.h"
#import "NSObject+MCO.h"

@implementation MCOAttachment {
    NSData * _data;
}

#define nativeType mailcore::Attachment

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::Attachment * attachment = (mailcore::Attachment *) object;
    return [[[self alloc] initWithMCPart:attachment] autorelease];
}

- (id) init
{
    mailcore::Attachment * attachment = new mailcore::Attachment();
    self = [self initWithMCPart:attachment];
    attachment->release();
    
    return self;
}

- (void) dealloc
{
    [_data release];
    [super dealloc];
}

+ (NSString *) mimeTypeForFilename:(NSString *)filename
{
    return [NSString mco_stringWithMCString:mailcore::Attachment::mimeTypeForFilename([filename mco_mcString])];
}

+ (MCOAttachment *) attachmentWithContentsOfFile:(NSString *)filename
{
    mailcore::Attachment * result = mailcore::Attachment::attachmentWithContentsOfFile([filename mco_mcString]);
    return MCO_TO_OBJC(result);
}

+ (MCOAttachment *) attachmentWithData:(NSData *)data filename:(NSString *)filename
{
    mailcore::Attachment * result = mailcore::Attachment::attachmentWithData([filename mco_mcString], [data mco_mcData]);
    return MCO_TO_OBJC(result);
}

+ (MCOAttachment *) attachmentWithHTMLString:(NSString *)htmlString
{
    mailcore::Attachment * result = mailcore::Attachment::attachmentWithHTMLString([htmlString mco_mcString]);
    return MCO_TO_OBJC(result);
}

+ (MCOAttachment *) attachmentWithRFC822Message:(NSData *)messageData
{
    mailcore::Attachment * result = mailcore::Attachment::attachmentWithRFC822Message([messageData mco_mcData]);
    return MCO_TO_OBJC(result);
}

+ (MCOAttachment *) attachmentWithText:(NSString *)text
{
    mailcore::Attachment * result = mailcore::Attachment::attachmentWithText([text mco_mcString]);
    return MCO_TO_OBJC(result);
}

MCO_OBJC_SYNTHESIZE_DATA(setData, data)

- (NSString *) decodedString
{
	mailcore::String *result = MCO_NATIVE_INSTANCE->decodedString();
	return [NSString mco_stringWithMCString:result];
}

+ (NSString *) decodedMIMEHeaderValue:(char *)mimestr
{
	mailcore::String *mcstr = mailcore::String::stringByDecodingMIMEHeaderValue(mimestr);
	return MCO_TO_OBJC(mcstr);
}

- (void) setContentTypeParameterValue:(NSString *)value forName:(NSString *)name
{
    MCO_NATIVE_INSTANCE->setContentTypeParameter(MCO_FROM_OBJC(mailcore::String, name), MCO_FROM_OBJC(mailcore::String, value));
}

- (NSString *) contentTypeParameterValueForName:(NSString *)name
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->contentTypeParameterValueForName((MCO_FROM_OBJC(mailcore::String, name))));
}
- (void) removeContentTypeParameterForName:(NSString *)name
{
    MCO_NATIVE_INSTANCE->removeContentTypeParameter(MCO_FROM_OBJC(mailcore::String, name));
}

- (NSArray * /* NSString */) allContentTypeParametersNames
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->allContentTypeParametersNames());
}

@end
