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

- (void) setExtraParameterValue:(NSString *)value forName:(NSString *)name
{
    MCO_NATIVE_INSTANCE->setExtraParameter(MCO_FROM_OBJC(mailcore::String, name), MCO_FROM_OBJC(mailcore::String, value));
}

- (NSString *) extraParameterValueForName:(NSString *)name
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->extraParameterValueForName((MCO_FROM_OBJC(mailcore::String, name))));
}
- (void) removeExtraParameterForName:(NSString *)name
{
    MCO_NATIVE_INSTANCE->removeExtraParameter(MCO_FROM_OBJC(mailcore::String, name));
}

- (NSArray * /* NSString */) allExtraParametersNames
{
    return MCO_TO_OBJC(MCO_NATIVE_INSTANCE->allExtraParametersNames());
}

@end
