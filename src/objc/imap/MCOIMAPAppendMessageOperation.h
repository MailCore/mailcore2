//
//  MCOIMAPAppendMessageOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPAPPENDMESSAGEOPERATION_H_

#define __MAILCORE_MCOIMAPAPPENDMESSAGEOPERATION_H_

#import <MailCore/MCOIMAPBaseOperation.h>
#import <MailCore/MCOConstants.h>

@interface MCOIMAPAppendMessageOperation : MCOIMAPBaseOperation

@property (nonatomic, copy) MCOIMAPBaseOperationProgressBlock progress;

- (void)start:(void (^)(NSError *error, uint32_t createdUID))completionBlock;

@end

#endif
