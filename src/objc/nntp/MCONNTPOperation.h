//
//  MCONNTPOperation.h
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCONNTPOPERATION_H

#define MAILCORE_MCONNTPOPERATION_H

#import <Foundation/Foundation.h>
#import <MailCore/MCOOperation.h>

/**
 This is a generic asynchronous NNTP3 operation. 
 @see MCONNTPSession
 */

NS_ASSUME_NONNULL_BEGIN
@interface MCONNTPOperation : MCOOperation

/** 
 Starts the asynchronous operation.
 
 @param completionBlock Called when the operation is finished.
 
 - On success `error` will be nil
 
 - On failure, `error` will be set with `MCOErrorDomain` as domain and an 
 error code available in MCOConstants.h,
 */
- (void) start:(void (^)(NSError * __nullable error))completionBlock;

@end
NS_ASSUME_NONNULL_END

#endif
