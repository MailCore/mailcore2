//
//  MCOIMAPIdleOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPIDLEOPERATION_H_

#define __MAILCORE_MCOIMAPIDLEOPERATION_H_

// This class implements an idle operation.

#import <MailCore/MCOIMAPBaseOperation.h>

@interface MCOIMAPIdleOperation : MCOIMAPBaseOperation

// Interrupts the IDLE operation.
- (void) interruptIdle;

// Starts the asynchronous operation.
// On success, the completion block will be called with nil as error.
// On failure, error will be set with MCOErrorDomain as domain and an error code available in MCOConstants.h.
- (void) start:(void (^)(NSError * error))completionBlock;

@end

#endif
