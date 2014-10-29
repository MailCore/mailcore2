//
//  MCOSMTPLoginOperation.h
//  mailcore2
//
//  Created by Robert Widmann on 9/24/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCOSMTPLOGINOPERATION_H

#define MAILCORE_MCOSMTPLOGINOPERATION_H

#import <MailCore/MCOSMTPOperation.h>

/** This is an asynchronous operation that will perform a noop operation through SMTP. */
@interface MCOSMTPLoginOperation : MCOSMTPOperation

/*
 Starts the asynchronous operation.
 
 @param completionBlock Called when the operation is finished.
 
 - On success `error` will be nil
 
 - On failure, `error` will be set with `MCOErrorDomain` as domain and an
 error code available in MCOConstants.h,
 */
- (void) start:(void (^)(NSError * error))completionBlock;

@end

#endif
