//
//  MCOCheckAccountOperation.m
//  mailcore2
//
//  Created by Matt Ronge on 01/31/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <mailcore/MCCore.h>

#import "MCOOperation+Internals.h"
#import "MCOCheckAccountOperation.h"
#import "NSError+MCO.h"

using namespace mailcore;

@implementation MCOCheckAccountOperation

- (void)operationCompleted {
    IMAPOperation *op = dynamic_cast<IMAPOperation *>(self.operation);
    NSError *error = [NSError mco_errorWithErrorCode:op->error()];
    self.completionBlock(error, self, nil); // No data to return
}

@end
