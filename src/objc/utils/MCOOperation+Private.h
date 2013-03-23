//
//  MCOOperation+Private.h
//  mailcore2
//
//  Created by Matt Ronge on 01/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifdef __cplusplus
namespace mailcore {
    class Operation;
}
#endif

// Shhh, secret stuff in here

@interface MCOOperation (Private)
#ifdef __cplusplus
- (id)initWithMCOperation:(mailcore::Operation *)op;
- (mailcore::Operation *)mcOperation;
#endif
- (void)start;
@end