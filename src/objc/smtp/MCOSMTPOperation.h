//
//  MCOSMTPCheckAccountOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOSMTPOPERATION_H_

#define __MAILCORE_MCOSMTPOPERATION_H_

#import <MailCore/MCOOperation.h>

// This is an asynchronous SMTP operation.

@interface MCOSMTPOperation : MCOOperation

// Starts the asynchronous operation.
// On success, the completion block will be called with nil as error.
// On failure, error will be set with MCOErrorDomain as domain and an error code available in MCOConstants.h.
- (void) start:(void (^)(NSError * error))completionBlock;

@end

#endif
