//
//  MCOIMAPIdentityOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPIDENTITYOPERATION_H_

#define __MAILCORE_MCOIMAPIDENTITYOPERATION_H_

// This class implements an operation to get/send identity.

#import <MailCore/MCOIMAPBaseOperation.h>

@interface MCOIMAPIdentityOperation : MCOIMAPBaseOperation

// Starts the asynchronous operation.
// On success, the completion block will be called with nil as error. serverIdentity contains the server identity information.
// See RFC 2971 for the common keys of this dictionary.
// On failure, error will be set with MCOErrorDomain as domain and an error code available in MCOConstants.h.
// serverIdentity will be nil.
- (void) start:(void (^)(NSError * error, NSDictionary * serverIdentity))completionBlock;

@end

#endif
