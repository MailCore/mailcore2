//
//  MCOIMAPCopyMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <MailCore/MCOIMAPBaseOperation.h>

#ifndef __MAILCORE_MCOCOPYMESSAGESOPERATION_H_

#define __MAILCORE_MCOCOPYMESSAGESOPERATION_H_

// This class implements an operation to copy messages.

@class MCOIndexSet;

@interface MCOIMAPCopyMessagesOperation : MCOIMAPBaseOperation

// Starts the asynchronous operation.
// On success, the completion block will be called with nil as error. destUids will contain the UID of the messages
// created in the destination folder if UIDPLUS is supported. destUids will be nil if UIDPLUS is not supported.
// On failure, error will be set with MCOErrorDomain as domain and an error code available in MCOConstants.h.
// info will be nil.
- (void) start:(void (^)(NSError * error, MCOIndexSet * destUids))completionBlock;

@end

#endif
