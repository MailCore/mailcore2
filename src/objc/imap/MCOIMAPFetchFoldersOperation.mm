//
//  MCOIMAPFetchFoldersOperation.m
//  mailcore2
//
//  Created by Matt Ronge on 1/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPFetchFoldersOperation.h"
#import "NSError+MCO.h"
#import "NSString+MCO.h"
#import "MCOOperation+Private.h"

#import <Foundation/Foundation.h>
#import <mailcore/MCAsync.h>

using namespace mailcore;

@interface MCOIMAPFetchFoldersOperation ()
@property (nonatomic, copy) void (^completionBlock)(NSError *error, NSArray *folder);
@end

@implementation MCOIMAPFetchFoldersOperation

- (void)start:(void (^)(NSError *error, NSArray *folder))completionBlock {
    self.completionBlock = completionBlock;
    [self start];
}

- (void)operationCompleted {
    IMAPFetchFoldersOperation *op = (IMAPFetchFoldersOperation *) [self mcOperation];
    if (op->error() == ErrorNone) {
        NSMutableArray *nsfolders = [NSMutableArray array];
        Array *folders = op->folders();
        for (int i = 0 ; i < folders->count(); i++) {
            NSString *folder = [NSString mco_stringWithMCObject:folders->objectAtIndex(i)];
            [nsfolders addObject:folder];
        }
        
        self.completionBlock(nil, nsfolders);
    } else {
        self.completionBlock([NSError mco_errorWithErrorCode:op->error()], nil);
    }
}
@end
