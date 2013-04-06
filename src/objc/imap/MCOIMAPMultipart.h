//
//  MCOIMAPMultipart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPMULTIPART_H_

#define __MAILCORE_MCOIMAPMULTIPART_H_

// This class implements a multipart.

#import <MailCore/MCOAbstractMultipart.h>

@interface MCOIMAPMultipart : MCOAbstractMultipart <NSCopying>

// It's the part identifier.
// A part identifier looks like 1.2.1
@property (nonatomic, copy) NSString * partID;

@end

#endif
