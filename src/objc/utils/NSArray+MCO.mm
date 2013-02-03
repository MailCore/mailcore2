//
//  NSArray+MCO.cpp
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "NSArray+MCO.h"

#include "MCBaseTypes.h"

#import "NSObject+MCO.h"

@implementation NSArray (MCO)

- (mailcore::Array *) mco_mcArray
{
    mailcore::Array * result = mailcore::Array::array();
    for(NSObject * value in self) {
        [value mco_mcObject];
    }
    return result;
}

@end
