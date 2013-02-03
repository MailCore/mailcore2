//
//  NSDictionary+MCO.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "NSDictionary+MCO.h"

#include "MCBaseTypes.h"

#import "NSObject+MCO.h"

@implementation NSDictionary (MCO)

- (mailcore::HashMap *) mco_mcHashMap
{
    mailcore::HashMap * result = mailcore::HashMap::hashMap();
    for(NSObject * key in self) {
        NSObject * value = [self objectForKey:key];
        result->setObjectForKey([key mco_mcObject], [value mco_mcObject]);
    }
    return result;
}

@end
