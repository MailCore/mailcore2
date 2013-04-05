//
//  MCOIMAPMessagePart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPMESSAGEPART_H_

#define __MAILCORE_MCOIMAPMESSAGEPART_H_

#import <MailCore/MCOAbstractMessagePart.h>

@interface MCOIMAPMessagePart : MCOAbstractMessagePart <NSCopying>

@property (nonatomic, copy) NSString * partID;

@end

#endif
