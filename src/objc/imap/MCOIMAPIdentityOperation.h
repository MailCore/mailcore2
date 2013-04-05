//
//  MCOIMAPIdentityOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPIDENTITYOPERATION_H_

#define __MAILCORE_MCOIMAPIDENTITYOPERATION_H_

#import <MailCore/MCOIMAPBaseOperation.h>

@interface MCOIMAPIdentityOperation : MCOIMAPBaseOperation

- (void)start:(void (^)(NSError *error, NSDictionary * serverIdentity))completionBlock;

@end

#endif
