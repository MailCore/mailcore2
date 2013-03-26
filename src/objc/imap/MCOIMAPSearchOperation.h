//
//  MCOIMAPSearchOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOOperation.h>

@class MCOIndexSet;

@interface MCOIMAPSearchOperation : MCOOperation

- (void)start:(void (^)(NSError *error, MCOIndexSet * searchResult))completionBlock;

@end
