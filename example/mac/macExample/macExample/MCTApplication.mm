//
//  MCTApplication.m
//  macExample
//
//  Created by DINH Viêt Hoà on 4/3/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCTApplication.h"

#import <mailcore/mailcore.h>

@implementation MCTApplication

- (void) sendEvent:(NSEvent *)theEvent
{
    mailcore::AutoreleasePool * pool = new mailcore::AutoreleasePool();
    [super sendEvent:theEvent];
    pool->release();
}

@end
