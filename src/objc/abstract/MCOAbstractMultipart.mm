//
//  MCOAbstractMultipart.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOAbstractMultipart.h"

#include "MCAbstractMultipart.h"

#import "NSObject+MCO.h"

@implementation MCOAbstractMultipart

- (NSArray *) parts
{
    return (NSArray *) [NSObject mco_objectWithMCObject:((mailcore::AbstractMultipart *) [self mco_mcObject])->parts()];
}

- (void) setParts:(NSArray *)parts
{
    ((mailcore::AbstractMultipart *) [self mco_mcObject])->setParts((mailcore::Array *) [parts mco_mcObject]);
}

@end
