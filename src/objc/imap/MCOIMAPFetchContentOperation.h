//
//  MCOIMAPFetchContentOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPFETCHCONTENTOPERATION_H_

#define __MAILCORE_MCOIMAPFETCHCONTENTOPERATION_H_

#import <MailCore/MCOIMAPBaseOperation.h>
#import <MailCore/MCOConstants.h>

@interface MCOIMAPFetchContentOperation : MCOIMAPBaseOperation

@property (nonatomic, copy) MCOIMAPBaseOperationProgressBlock progress;

- (void)start:(void (^)(NSError *error, NSData * data))completionBlock;

@end

#endif
