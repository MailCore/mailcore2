//
//  MCOAbstractMultipart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOABSTRACTMULTIPART_H_

#define __MAILCORE_MCOABSTRACTMULTIPART_H_

#import <Foundation/Foundation.h>
#import <mailcore/MCOAbstractPart.h>

@interface MCOAbstractMultipart : MCOAbstractPart

@property (nonatomic, copy) NSArray * parts;

@end

#endif
