//
//  MCOFetchFoldersOperation.m
//  mailcore2
//
//  Created by Matt Ronge on 1/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOFetchFoldersOperation.h"
#import "NSError+MCO.h"
#import "NSString+MCO.h"
#import "MCOOperation+Internals.h"

#import <Foundation/Foundation.h>
#import <mailcore/MCAsync.h>

using namespace mailcore;

@implementation MCOFetchFoldersOperation
- (void)operationCompleted {
    IMAPFetchFoldersOperation *op = dynamic_cast<IMAPFetchFoldersOperation *>(self.operation);
    if (op->error() == ErrorNone) {
        NSMutableArray *nsfolders = [NSMutableArray array];
        Array *folders = op->folders();
        for (int i = 0 ; i < folders->count(); folders++) {
            NSString *folder = [NSString mco_stringWithMCObject:folders->objectAtIndex(i)];
            [nsfolders addObject:folder];
        }
        
        self.completionBlock(nil, self, nsfolders);
    } else {
        self.completionBlock([NSError mco_errorWithErrorCode:op->error()], self, nil);
    }
}
@end
