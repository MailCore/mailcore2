//
//  MCOIMAPSearchOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOSEARCHOPERATION_H_

#define __MAILCORE_MCOSEARCHOPERATION_H_

#import <MailCore/MCOIMAPBaseOperation.h>

@class MCOIndexSet;

@interface MCOIMAPSearchOperation : MCOIMAPBaseOperation

- (void)start:(void (^)(NSError *error, MCOIndexSet * searchResult))completionBlock;

@end

#endif
