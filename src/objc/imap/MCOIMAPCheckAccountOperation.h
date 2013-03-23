//
//  MCOIMAPCheckAccountOperation.h
//  mailcore2
//
//  Created by Matt Ronge on 01/31/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPCHECKACCOUNTOPERATION_H_

#define __MAILCORE_MCOIMAPCHECKACCOUNTOPERATION_H_

#import <mailcore/MCOOperation.h>

@interface MCOIMAPCheckAccountOperation : MCOOperation
- (void)start:(void (^)(NSError *error))completionBlock;
@end

#endif
