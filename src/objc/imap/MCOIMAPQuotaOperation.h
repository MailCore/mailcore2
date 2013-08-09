//
//  MCOIMAPQuotaOperation.h
//  mailcore2
//
//  Created by Petro Korenev on 8/2/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPBaseOperation.h"

@interface MCOIMAPQuotaOperation : MCOIMAPBaseOperation

- (void) start:(void (^)(NSError * error, NSUInteger usage, NSUInteger limit))completionBlock;

@end
