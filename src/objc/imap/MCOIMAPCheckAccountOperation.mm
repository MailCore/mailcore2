//
//  MCOIMAPCheckAccountOperation.m
//  mailcore2
//
//  Created by Matt Ronge on 01/31/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import "MCOIMAPCheckAccountOperation.h"

#import "MCAsync.h"

#import "MCOOperation+Private.h"
#import "MCOIMAPCheckAccountOperation.h"
#import "NSError+MCO.h"

using namespace mailcore;

@interface MCOIMAPCheckAccountOperation ()
@property (nonatomic, copy) void (^completionBlock)(NSError *error);
@end

@implementation MCOIMAPCheckAccountOperation

- (void)start:(void (^)(NSError *error))completionBlock {
    self.completionBlock = completionBlock;
    [self start];
}

- (void)operationCompleted {
    IMAPOperation *op = (IMAPOperation *) [self mcOperation];
    NSError *error = [NSError mco_errorWithErrorCode:op->error()];
    self.completionBlock(error);
}

@end
