//
//  MCOAbstractMessagePart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOABSTRACTMESSAGEPART_H_

#define __MAILCORE_MCOABSTRACTMESSAGEPART_H_

#import <Foundation/Foundation.h>
#import <MailCore/MCOAbstractPart.h>

@class MCOMessageHeader;

@interface MCOAbstractMessagePart : MCOAbstractPart <NSCoding>

// Returns the header of the embedded message.
@property (nonatomic, strong) MCOMessageHeader * header;

// Returns the main part of the embedded message. It can be MCOAbstractPart, MCOAbstractMultipart
// or a MCOAbstractMessagePart.
@property (nonatomic, strong) MCOAbstractPart * mainPart;

@end

#endif

