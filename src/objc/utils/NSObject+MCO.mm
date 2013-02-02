//
//  NSObject+MCO.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "NSObject+MCO.h"

#import "NSData+MCO.h"
#import "NSString+MCO.h"
#import "NSDictionary+MCO.h"
#import "NSArray+MCO.h"

#include "MCBaseTypes.h"
#include "MCUtils.h"

@implementation NSObject (MCO)

- (mailcore::Object *) mco_mcObject
{
    if ([self isKindOfClass:[NSData class]]) {
        return [(NSData *) self mco_mcData];
    }
    else if ([self isKindOfClass:[NSString class]]) {
        return [(NSString *) self mco_mcString];
    }
    else if ([self isKindOfClass:[NSArray class]]) {
        return [(NSArray *) self mco_mcArray];
    }
    else if ([self isKindOfClass:[NSDictionary class]]) {
        return [(NSDictionary *) self mco_mcHashMap];
    }
    else {
        MCAssert(0);
        return nil;
    }
    
}

@end
