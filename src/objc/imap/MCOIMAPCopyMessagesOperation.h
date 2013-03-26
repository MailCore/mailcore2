//
//  MCOIMAPCopyMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOOperation.h>

@class MCOIndexSet;

@interface MCOIMAPCopyMessagesOperation : MCOOperation

/*
@property (nonatomic, copy) NSString * folder;
@property (nonatomic, strong) MCOIndexSet * uids;
@property (nonatomic, copy) NSString * destFolder;
*/

- (void)start:(void (^)(NSError *error, MCOIndexSet * destUids))completionBlock;

@end
