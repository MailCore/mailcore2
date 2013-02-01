//
//  MCOOperation+Internals.h
//  mailcore2
//
//  Created by Matt Ronge on 01/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOperation.h>
#import <mailcore/MCOOperation.h>

// Shhh, secret stuff in here

@interface MCOOperation ()
- (id)initWithOperation:(mailcore::Operation *)op;
- (mailcore::Operation *)operation;
@end