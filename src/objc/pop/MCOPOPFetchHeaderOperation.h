//
//  MCOFetchHeaderOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOPOPFETCHHEADEROPERATION_H_

#define __MAILCORE_MCOPOPFETCHHEADEROPERATION_H_

#import <Foundation/Foundation.h>
#import <mailcore/MCOPOPOperation.h>

@class MCOMessageHeader;

@interface MCOPOPFetchHeaderOperation : MCOPOPOperation
- (void)start:(void (^)(NSError *error, MCOMessageHeader * header))completionBlock;
@end

#endif
