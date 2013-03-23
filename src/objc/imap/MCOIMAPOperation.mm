//
//  MCOIMAPOperation.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPOperation.h"

#include "MCIMAPOperation.h"
#import "MCOOperation+Private.h"

#import "MCOUtils.h"

@interface MCOIMAPOperation ()
@property (nonatomic, copy) void (^completionBlock)(NSError *error);
@end

@implementation MCOIMAPOperation {
    void (^_completionBlock)(NSError *error);
}

@synthesize completionBlock = _completionBlock;

#define nativeType mailcore::IMAPOperation

+ (void) initialize
{
    MCORegisterClass(self, &typeid(nativeType));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::IMAPOperation * op = (mailcore::IMAPOperation *) object;
    return [[[self alloc] initWithMCOperation:op] autorelease];
}

- (void)start:(void (^)(NSError *error))completionBlock {
    self.completionBlock = completionBlock;
    [self start];
}

- (void)operationCompleted {
    NSError * error = [NSError mco_errorWithErrorCode:MCO_NATIVE_INSTANCE->error()];
    self.completionBlock(error);
}

@end
