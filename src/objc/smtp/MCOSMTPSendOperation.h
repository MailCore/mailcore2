//
//  MCOSMTPSendOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOSMTPSENDOPERATION_H_

#define __MAILCORE_MCOSMTPSENDOPERATION_H_

#import <mailcore/MCOSMTPOperation.h>

typedef void (^MCOSMTPOperationProgressBlock)(unsigned int current, unsigned int maximum);

@interface MCOSMTPSendOperation : MCOSMTPOperation

@property (nonatomic, copy) MCOSMTPOperationProgressBlock progress;

- (void)start:(void (^)(NSError *error))completionBlock;

@end

#endif
