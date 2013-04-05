//
//  MCODeleteMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOPOPOPERATION_H_

#define __MAILCORE_MCOPOPOPERATION_H_

#import <Foundation/Foundation.h>
#import <MailCore/MCOOperation.h>

// This is an asynchronous POP3 operation.
// See MCOPOPSession for more info.

@interface MCOPOPOperation : MCOOperation
- (void)start:(void (^)(NSError *error))completionBlock;
@end

#endif
