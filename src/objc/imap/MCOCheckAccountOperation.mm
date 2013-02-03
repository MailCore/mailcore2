//
//  MCOCheckAccountOperation.m
//  mailcore2
//
//  Created by Matt Ronge on 01/31/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <mailcore/MCAsync.h>

#import "MCOOperation+Internals.h"
#import "MCOCheckAccountOperation.h"
#import "NSError+MCO.h"

using namespace mailcore;

@interface MCOCheckAccountOperation ()
@property (nonatomic, copy) void (^completionBlock)(NSError *error);
@end

@implementation MCOCheckAccountOperation

- (void)start:(void (^)(NSError *error))completionBlock {
    self.completionBlock = completionBlock;
    [self start];
}

- (void)operationCompleted {
    IMAPOperation *op = dynamic_cast<IMAPOperation *>(self.operation);
    NSError *error = [NSError mco_errorWithErrorCode:op->error()];
    self.completionBlock(error);
}

@end
