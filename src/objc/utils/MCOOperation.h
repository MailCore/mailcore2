//
//  MCOOperation.h
//  mailcore2
//
//  Created by Matt Ronge on 01/31/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef __MAILCORE_MCOOPERATION_H_

#define __MAILCORE_MCOOPERATION_H_

#import <Foundation/Foundation.h>

@interface MCOOperation : NSObject

/** Returns whether the operation is cancelled.*/
@property (readonly) BOOL isCancelled;

/** The queue this operation dispatches the callback on.  Defaults to the main queue.*/
@property (nonatomic, assign) dispatch_queue_t callbackDispatchQueue;

/** This methods is called on the main thread when the asynchronous operation is finished.
 Needs to be overriden by subclasses.*/
- (void) operationCompleted;

/** Cancel the operation.*/
- (void) cancel;


@end

#endif
