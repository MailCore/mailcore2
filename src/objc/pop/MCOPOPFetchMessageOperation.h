//
//  MCOFetchMessageOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOPOPFETCHMESSAGEOPERATION_H_

#define __MAILCORE_MCOPOPFETCHMESSAGEOPERATION_H_

#import <Foundation/Foundation.h>
#import <mailcore/MCOPOPOperation.h>

typedef void (^MCOPOPOperationProgressBlock)(unsigned int current, unsigned int maximum);

@interface MCOPOPFetchMessageOperation : MCOPOPOperation

@property (nonatomic, copy) MCOPOPOperationProgressBlock progress;

- (void)start:(void (^)(NSError *error, NSData * messageData))completionBlock;

@end

#endif
