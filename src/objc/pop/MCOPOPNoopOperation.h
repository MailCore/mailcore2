//
//  MCOPOPNoopOperation.h
//  mailcore2
//
//  Created by Robert Widmann on 9/24/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOPOPNOOPOPERATION_H_

#define __MAILCORE_MCOPOPNOOPOPERATION_H_

#import <Foundation/Foundation.h>
#import <MailCore/MCOPOPOperation.h>

/** This is an asynchronous operation that will perform a No-Op on the POP3 account. */

@interface MCOPOPNoopOperation : MCOPOPOperation

/**
 Starts the asynchronous noop operation.
 
 @param completionBlock Called when the operation is finished.
 
 - On success `error` will be nil
 
 - On failure, `error` will be set with `MCOErrorDomain` as domain and an
 error code available in MCOConstants.h
 */
- (void) start:(void (^)(NSError * error))completionBlock;

@end

#endif
