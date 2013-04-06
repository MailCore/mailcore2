//
//  MCOOperation+Private.h
//  mailcore2
//
//  Created by Matt Ronge on 01/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOPERATION_PRIVATE_H_

#define __MAILCORE_MCOPERATION_PRIVATE_H_

#ifdef __cplusplus
namespace mailcore {
    class Operation;
}
#endif

// Shhh, secret stuff in here

@interface MCOOperation (Private)
#ifdef __cplusplus
- (id) initWithMCOperation:(mailcore::Operation *)op;
#endif
- (void) start;
@end

#endif
