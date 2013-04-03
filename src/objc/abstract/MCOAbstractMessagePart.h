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

@interface MCOAbstractMessagePart : MCOAbstractPart

@property (nonatomic, strong) MCOMessageHeader * header;
@property (nonatomic, strong) MCOAbstractPart * mainPart;

@end

#endif

