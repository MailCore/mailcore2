//
//  MCOIMAPFolderInfoOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOOperation.h>
#import <mailcore/MCOIMAPOperation.h>

@class MCOIMAPFolderInfo;

@interface MCOIMAPFolderInfoOperation : MCOIMAPOperation
- (void)start:(void (^)(NSError *error, MCOIMAPFolderInfo *info))completionBlock;
@end
