//
//  MCOIMAPFolderInfoOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOIMAPBaseOperation.h>

#ifndef __MAILCORE_MCOIMAPFOLDERINFOOPERATION_H_

#define __MAILCORE_MCOIMAPFOLDERINFOOPERATION_H_

@class MCOIMAPFolderInfo;

@interface MCOIMAPFolderInfoOperation : MCOIMAPBaseOperation
- (void)start:(void (^)(NSError *error, MCOIMAPFolderInfo *info))completionBlock;
@end

#endif
