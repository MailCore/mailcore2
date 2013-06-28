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
#import <MailCore/MCOAbstractPart.h>

@interface MCOAbstractMultipart : MCOAbstractPart

/** Returns the subparts of that multipart.*/
@property (nonatomic, copy) NSArray * /* MCOAbstractPart */ parts;

@end

#endif
