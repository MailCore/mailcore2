//
//  MCOIMAPAppendMessageOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOIMAPBaseOperation.h>
#import <mailcore/MCOConstants.h>

#ifndef __MAILCORE_MCOIMAPAPPENDMESSAGEOPERATION_H_

#define __MAILCORE_MCOIMAPAPPENDMESSAGEOPERATION_H_

@interface MCOIMAPAppendMessageOperation : MCOIMAPBaseOperation

@property (nonatomic, copy) MCOIMAPBaseOperationProgressBlock progress;

- (void)start:(void (^)(NSError *error, uint32_t createdUID))completionBlock;

@end

#endif
