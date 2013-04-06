//
//  MCOIMAPFetchContentOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPFETCHCONTENTOPERATION_H_

#define __MAILCORE_MCOIMAPFETCHCONTENTOPERATION_H_

// This class implements an operation to fetch the content of a message.
// It can be a part or a full message.

#import <MailCore/MCOIMAPBaseOperation.h>
#import <MailCore/MCOConstants.h>

@interface MCOIMAPFetchContentOperation : MCOIMAPBaseOperation

// This block will be called during the progression while receiving the message
// when some bytes have been downloaded from the network.
@property (nonatomic, copy) MCOIMAPBaseOperationProgressBlock progress;

// Starts the asynchronous operation.
// On success, the completion block will be called with nil as error. data contains the requested data of the message.
// On failure, error will be set with MCOErrorDomain as domain and an error code available in MCOConstants.h.
// data will be nil.
- (void) start:(void (^)(NSError * error, NSData * data))completionBlock;

@end

#endif
