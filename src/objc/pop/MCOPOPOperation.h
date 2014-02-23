//
//  MCODeleteMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCOPOPOPERATION_H

#define MAILCORE_MCOPOPOPERATION_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOOperation.h>

/**
 This is a generic asynchronous POP3 operation. 
 @see MCOPOPSession
*/

@interface MCOPOPOperation : MCOOperation

/** 
 Starts the asynchronous operation.

 @param completionBlock Called when the operation is finished.

 - On success `error` will be nil
 
 - On failure, `error` will be set with `MCOErrorDomain` as domain and an 
   error code available in MCOConstants.h,
*/
- (void) start:(void (^)(NSError * error))completionBlock;

@end

#endif
