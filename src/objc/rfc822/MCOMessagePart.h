//
//  MessagePart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOMESSAGEPART_H_

#define __MAILCORE_MCOMESSAGEPART_H_

#import <MailCore/MCOAbstractMessagePart.h>

/** Message part parsed from RFC 822 message data. */

@interface MCOMessagePart : MCOAbstractMessagePart <NSCopying>

@end

#endif
