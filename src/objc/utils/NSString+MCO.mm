//
//  NSString+MCO.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/21/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "NSString+MCO.h"

@implementation NSString (MCO)

+ (NSString *) mco_stringWithMCString:(mailcore::String *)cppString
{
    if (cppString == NULL)
        return nil;
    
    return [NSString stringWithCharacters:(const unichar *) cppString->unicodeCharacters() length:cppString->length()];
}

+ (NSString *) mco_stringWithMCObject:(mailcore::Object *)object
{
    if (object == NULL)
        return nil;
    
    return [NSString mco_stringWithMCString:object->description()];
}

@end
