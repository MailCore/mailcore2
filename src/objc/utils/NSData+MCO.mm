//
//  NSData+MCO.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/21/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "NSData+MCO.h"

#include "MCData.h"
#include "MCString.h"
#import "NSString+MCO.h"

@implementation NSData (MCO)

+ (id) mco_objectWithMCObject:(mailcore::Object *)object
{
    return [self mco_dataWithMCData:(mailcore::Data *) object];
}

+ (NSData *) mco_dataWithMCData:(mailcore::Data *)cppData
{
    if (cppData == NULL)
        return nil;
    
    return [NSData dataWithBytes:cppData->bytes() length:cppData->length()];
}

- (mailcore::Data *) mco_mcData
{
    return mailcore::Data::dataWithBytes((const char *) [self bytes], (unsigned int) [self length]);
}

- (NSString *) stringWithDetectedCharset
{
	return [NSString mco_stringWithMCString:self.mco_mcData->stringWithDetectedCharset()];
}

- (NSString *) stringWithDetectedCharset:(const char *)hintCharset
{
	return [NSString mco_stringWithMCString:self.mco_mcData->stringWithDetectedCharset(mailcore::String::stringWithUTF8Characters(hintCharset), false)];
}

- (NSString *) stringWithDetectedCharset:(const char *)charset isHTML:(BOOL)isHTML
{
	return [NSString mco_stringWithMCString:self.mco_mcData->stringWithDetectedCharset(mailcore::String::stringWithUTF8Characters(charset), isHTML)];
}

@end
