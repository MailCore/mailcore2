//
//  MCOIMAPCapabilityOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPCAPABILITYOPERATION_H_

#define __MAILCORE_MCOIMAPCAPBILITYOPERATION_H_

#import <MailCore/MCOIMAPBaseOperation.h>

@class MCOIndexSet;

@interface MCOIMAPCapabilityOperation : MCOIMAPBaseOperation

- (void)start:(void (^)(NSError *error, MCOIndexSet * capabilities))completionBlock;

@end

#endif
