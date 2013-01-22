//
//  NSData+MCO.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/21/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "NSData+MCO.h"

@implementation NSData (MCO)

+ (NSData *) mco_dataWithMCData:(mailcore::Data *)cppData
{
    if (cppData == NULL)
        return nil;
    
    return [NSData dataWithBytes:cppData->bytes() length:cppData->length()];
}

@end
