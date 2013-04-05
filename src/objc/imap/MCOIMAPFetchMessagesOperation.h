//
//  MCOIMAPFetchMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPFETCHMESSAGESOPERATION_H_

#define __MAILCORE_MCOIMAPFETCHMESSAGESOPERATION_H_

#import <MailCore/MCOIMAPBaseOperation.h>
#import <MailCore/MCOConstants.h>

@class MCOIndexSet;

@interface MCOIMAPFetchMessagesOperation : MCOIMAPBaseOperation

@property (nonatomic, copy) MCOIMAPBaseOperationItemProgressBlock progress;

- (void)start:(void (^)(NSError *error, NSArray * messages, MCOIndexSet * vanishedMessages))completionBlock;

@end

#endif
