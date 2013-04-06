//
//  MCOPOPFetchMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOPOPFETCHMESSAGESOPERATION_H_

#define __MAILCORE_MCOPOPFETCHMESSAGESOPERATION_H_

#import <Foundation/Foundation.h>
#import <MailCore/MCOPOPOperation.h>

// This is an asynchronous operation that will fetch the list of a messages on the account.

@interface MCOPOPFetchMessagesOperation : MCOPOPOperation

// Starts the asynchronous operation.
// On success, the completion block will be called with nil as error and the fetched messages list.
// Messages will be an array of MCOPOPMessageInfo.
// On failure, error will be set with MCOErrorDomain as domain and an error code available in MCOConstants.h.
// messageData will be nil.
- (void) start:(void (^)(NSError * error, NSArray * /* MCOPOPMessageInfo */ messages))completionBlock;

@end

#endif
