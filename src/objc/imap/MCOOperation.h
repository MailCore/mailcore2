//
//  MCOOperation.h
//  mailcore2
//
//  Created by Matt Ronge on 01/31/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MCOOperation;

typedef void (^MCOCompletionBlock)(NSError *error, MCOOperation *op, id response);

@interface MCOOperation : NSObject
@property (copy) MCOCompletionBlock completionBlock;
@property (readonly) BOOL isCancelled;

- (void)cancel;
- (void)start;
@end
