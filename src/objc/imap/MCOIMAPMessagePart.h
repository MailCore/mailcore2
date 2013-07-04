//
//  MCOIMAPMessagePart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPMESSAGEPART_H_

#define __MAILCORE_MCOIMAPMESSAGEPART_H_

/** Represents a message part. */

#import <MailCore/MCOAbstractMessagePart.h>

@interface MCOIMAPMessagePart : MCOAbstractMessagePart

/** A part identifier is of the form 1.2.1*/
@property (nonatomic, copy) NSString * partID;

@end

#endif
