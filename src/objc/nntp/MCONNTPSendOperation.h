//
//  MCONNTPSendOperation.h
//  mailcore2
//
//  Created by Daryle Walker on 2/21/16.
//  Copyright © 2016 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCONNTPSENDOPERATION_H

#define MAILCORE_MCONNTPSENDOPERATION_H

#import <Foundation/Foundation.h>
#import <MailCore/MCONNTPOperation.h>

/** Send a message from NNTP3 */

typedef void (^MCONNTPOperationProgressBlock)(unsigned int current, unsigned int maximum);

NS_ASSUME_NONNULL_BEGIN
@interface MCONNTPSendOperation : MCONNTPOperation

/** This block will be called as data is downloaded from the network */
@property (nonatomic, copy) MCONNTPOperationProgressBlock progress;

/**
 Starts the asynchronous send operation.
 
 @param completionBlock Called when the operation is finished.
 
 - On success `error` will be nil
 
 - On failure, `error` will be set with `MCOErrorDomain` as domain and an
 error code available in MCOConstants.h
 */
- (void) start:(void (^)(NSError * __nullable error))completionBlock;

@end
NS_ASSUME_NONNULL_END

#endif
