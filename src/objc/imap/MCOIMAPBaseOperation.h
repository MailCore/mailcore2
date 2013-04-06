//
//  MCOIMAPBaseOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/26/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPBASEOPERATION_H_

#define __MAILCORE_MCOIMAPBASEOPERATION_H_

// It's a generic IMAP operation with progress.

#import <MailCore/MCOOperation.h>

typedef void (^MCOIMAPBaseOperationProgressBlock)(unsigned int current, unsigned int maximum);
typedef void (^MCOIMAPBaseOperationItemProgressBlock)(unsigned int current);

@interface MCOIMAPBaseOperation : MCOOperation

// those two methods can be overriden by subclasses.

// It will be called when a send or receive of a content of a message will occur.
- (void) bodyProgress:(unsigned int)current maximum:(unsigned int)maximum;

// It will be called when a new item is received in a list of items.
- (void) itemProgress:(unsigned int)current maximum:(unsigned int)maximum;

@end

#endif
