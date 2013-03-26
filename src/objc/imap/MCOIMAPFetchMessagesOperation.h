//
//  MCOIMAPFetchMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOOperation.h>
#import <mailcore/MCOConstants.h>

@class MCOIndexSet;

@interface MCOIMAPFetchMessagesOperation : MCOOperation

/*
@property (nonatomic, assign, getter=isFetchByUidEnabled) BOOL fetchByUidEnabled;
@property (nonatomic, strong) MCOIndexSet * indexes;
@property (nonatomic, assign) uint64_t modSequenceValue;
@property (nonatomic, assign) MCOIMAPMessagesRequestKind kind;
*/

- (void)start:(void (^)(NSError *error, NSArray * messages, MCOIndexSet * vanishedMessages))completionBlock;

@end
