//
//  MCOSMTPSendOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOSMTPSENDOPERATION_H_

#define __MAILCORE_MCOSMTPSENDOPERATION_H_

#import <MailCore/MCOSMTPOperation.h>

// This is an asynchronous operation that will send a message through SMTP.

typedef void (^MCOSMTPOperationProgressBlock)(unsigned int current, unsigned int maximum);

@interface MCOSMTPSendOperation : MCOSMTPOperation

// This block will be called during the progression of the send of the message
// when some bytes have been sent to the network.
@property (nonatomic, copy) MCOSMTPOperationProgressBlock progress;

- (void)start:(void (^)(NSError *error))completionBlock;

@end

#endif
