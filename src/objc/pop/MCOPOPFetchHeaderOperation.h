//
//  MCOFetchHeaderOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOPOPFETCHHEADEROPERATION_H_

#define __MAILCORE_MCOPOPFETCHHEADEROPERATION_H_

#import <Foundation/Foundation.h>
#import <MailCore/MCOPOPOperation.h>

// This is an asynchronous operation that will fetch the header of a message.
// See MCOPOPSession for more info.

@class MCOMessageHeader;

@interface MCOPOPFetchHeaderOperation : MCOPOPOperation

// Starts the asynchronous operation.
// On success, the completion block will be called with nil as error and the content of the fetched header.
// On failure, error will be set with MCOErrorDomain as domain and an error code available in MCOConstants.h.
// header will be nil.
- (void) start:(void (^)(NSError * error, MCOMessageHeader * header))completionBlock;

@end

#endif
