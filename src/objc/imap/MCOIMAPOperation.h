//
//  MCOIMAPOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPOPERATION_H_

#define __MAILCORE_MCOIMAPOPERATION_H_

#import <MailCore/MCOOperation.h>

@interface MCOIMAPOperation : MCOOperation

- (void)start:(void (^)(NSError *error))completionBlock;

@end

#endif
