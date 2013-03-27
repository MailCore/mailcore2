//
//  MCOIMAPSearchOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOIMAPBaseOperation.h>

#ifndef __MAILCORE_MCOSEARCHOPERATION_H_

#define __MAILCORE_MCOSEARCHOPERATION_H_

@class MCOIndexSet;

@interface MCOIMAPSearchOperation : MCOIMAPBaseOperation

- (void)start:(void (^)(NSError *error, MCOIndexSet * searchResult))completionBlock;

@end

#endif
