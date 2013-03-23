//
//  MCOIMAPCheckAccountOperation.h
//  mailcore2
//
//  Created by Matt Ronge on 01/31/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <mailcore/MCOOperation.h>

@interface MCOCheckAccountOperation : MCOOperation
- (void)start:(void (^)(NSError *error))completionBlock;
@end
