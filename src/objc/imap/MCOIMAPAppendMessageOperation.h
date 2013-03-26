//
//  MCOIMAPAppendMessageOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOOperation.h>
#import <mailcore/MCOConstants.h>

@interface MCOIMAPAppendMessageOperation : MCOOperation

/*
@property (nonatomic, copy) NSString * folder;
@property (nonatomic, strong) NSData * messageData;
@property (nonatomic, assign) MCOMessageFlag flags;
*/

- (void)start:(void (^)(NSError *error, uint32_t createdUID))completionBlock;

@end
