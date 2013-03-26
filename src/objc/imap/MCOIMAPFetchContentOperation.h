//
//  MCOIMAPFetchContentOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOOperation.h>
#import <mailcore/MCOConstants.h>

@interface MCOIMAPFetchContentOperation : MCOOperation

/*
@property (nonatomic, assign) uint32_t uid;
@property (nonatomic, copy) NSString * partID;
@property (nonatomic, assign) MCOEncoding encoding;
*/

- (void)start:(void (^)(NSError *error, NSData * data))completionBlock;

@end
