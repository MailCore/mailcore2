//
//  MCOIMAPCopyMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOIMAPBaseOperation.h>

#ifndef __MAILCORE_MCOCOPYMESSAGESOPERATION_H_

#define __MAILCORE_MCOCOPYMESSAGESOPERATION_H_

@class MCOIndexSet;

@interface MCOIMAPCopyMessagesOperation : MCOIMAPBaseOperation

- (void)start:(void (^)(NSError *error, MCOIndexSet * destUids))completionBlock;

@end

#endif
