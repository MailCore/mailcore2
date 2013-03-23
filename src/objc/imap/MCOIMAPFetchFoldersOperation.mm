//
//  MCOIMAPFetchFoldersOperation.m
//  mailcore2
//
//  Created by Matt Ronge on 1/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPFetchFoldersOperation.h"

#import "NSError+MCO.h"
#import "MCOOperation+Private.h"

#import "MCOUtils.h"

#import <Foundation/Foundation.h>
#import <mailcore/MCAsync.h>

using namespace mailcore;

@interface MCOIMAPFetchFoldersOperation ()
@property (nonatomic, copy) void (^completionBlock)(NSError *error, NSArray *folder);
@end

@implementation MCOIMAPFetchFoldersOperation {
    void (^_completionBlock)(NSError *error, NSArray *folder);
}

@synthesize completionBlock = _completionBlock;

#define nativeType mailcore::IMAPFetchFoldersOperation

+ (void) initialize
{
    MCORegisterClass(self, &typeid(nativeType));
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::IMAPOperation * op = (mailcore::IMAPOperation *) object;
    return [[[self alloc] initWithMCOperation:op] autorelease];
}

- (void)start:(void (^)(NSError *error, NSArray *folder))completionBlock {
    self.completionBlock = completionBlock;
    [self start];
}

- (void)operationCompleted {
    IMAPFetchFoldersOperation *op = MCO_NATIVE_INSTANCE;
    if (op->error() == ErrorNone) {
        self.completionBlock(nil, MCO_TO_OBJC(op->folders()));
    } else {
        self.completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
}

@end
