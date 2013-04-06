//
//  MCOIMAPCapabilityOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPCAPABILITYOPERATION_H_

#define __MAILCORE_MCOIMAPCAPBILITYOPERATION_H_

// This class implements an operation to query capabilities.

#import <MailCore/MCOIMAPBaseOperation.h>

@class MCOIndexSet;

@interface MCOIMAPCapabilityOperation : MCOIMAPBaseOperation

// Starts the asynchronous operation.
// On success, the completion block will be called with nil as error. capabilities is a set of IMAP capabilities.
// See MCOConstants.h / MCOIMAPCapability.
// On failure, error will be set with MCOErrorDomain as domain and an error code available in MCOConstants.h.
// info will be nil.
- (void) start:(void (^)(NSError * error, MCOIndexSet * capabilities))completionBlock;

@end

#endif
