//
//  MCOIMAPNoopOperation.h
//  mailcore2
//
//  Created by Robert Widmann on 9/24/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPNOOPOPERATION_H_

#define __MAILCORE_MCOIMAPNOOPOPERATION_H_

#import <MailCore/MCOIMAPBaseOperation.h>

/* The class is used to perform a No-Op operation. */
@interface MCOIMAPNoopOperation : MCOIMAPBaseOperation

/**
 Starts the asynchronous operation.
 
 @param completionBlock Called when the operation is finished.
 
 - On success `error` will be nil
 
 - On failure, `error` will be set
 */

- (void) start:(void (^)(NSError * error))completionBlock;

@end

#endif
