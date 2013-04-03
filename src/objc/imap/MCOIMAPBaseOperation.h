//
//  MCOIMAPBaseOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/26/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <MailCore/MCOOperation.h>

#ifndef __MAILCORE_MCOIMAPBASEOPERATION_H_

#define __MAILCORE_MCOIMAPBASEOPERATION_H_

typedef void (^MCOIMAPBaseOperationProgressBlock)(unsigned int current, unsigned int maximum);
typedef void (^MCOIMAPBaseOperationItemProgressBlock)(unsigned int current);

@interface MCOIMAPBaseOperation : MCOOperation

// to be overriden by subclasses.
- (void) bodyProgress:(unsigned int)current maximum:(unsigned int)maximum;
- (void) itemProgress:(unsigned int)current maximum:(unsigned int)maximum;

@end

#endif
