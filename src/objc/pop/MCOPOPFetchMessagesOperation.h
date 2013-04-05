//
//  MCOPOPFetchMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOPOPFETCHMESSAGESOPERATION_H_

#define __MAILCORE_MCOPOPFETCHMESSAGESOPERATION_H_

#import <Foundation/Foundation.h>
#import <MailCore/MCOPOPOperation.h>

// This is an asynchronous operation that will fetch the list of a messages on the account.

@interface MCOPOPFetchMessagesOperation : MCOPOPOperation

- (void)start:(void (^)(NSError *error, NSArray * messages))completionBlock;

@end

#endif
