//
//  MCOIMAPFetchMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPFETCHMESSAGESOPERATION_H_

#define __MAILCORE_MCOIMAPFETCHMESSAGESOPERATION_H_

#import <MailCore/MCOIMAPBaseOperation.h>
#import <MailCore/MCOConstants.h>

// This class implements an operation to fetch a list of messages.

@class MCOIndexSet;

@interface MCOIMAPFetchMessagesOperation : MCOIMAPBaseOperation

// This block will be called each time a new message information will be downloaded.
@property (nonatomic, copy) MCOIMAPBaseOperationItemProgressBlock progress;

// Starts the asynchronous operation.
// On success, the completion block will be called with nil as error. messages will be an array of MCOIMAPMessage.
// vanishedMessages will contain the messages removed on the server if the server supports QRESYNC and if it was a sync request.
// In other case, vanishedMessages will be nil.
// On failure, error will be set with MCOErrorDomain as domain and an error code available in MCOConstants.h.
// messages and vanishedMessages will be nil.
- (void) start:(void (^)(NSError * error, NSArray * /* MCOIMAPMessage */ messages, MCOIndexSet * vanishedMessages))completionBlock;

@end

#endif
