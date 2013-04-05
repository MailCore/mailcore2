//
//  MCOIMAPPart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPABSTRACTPART_H_

#define __MAILCORE_MCOIMAPABSTRACTPART_H_

#import <MailCore/MCOAbstractPart.h>

#import <MailCore/MCOConstants.h>

@interface MCOIMAPPart : MCOAbstractPart <NSCopying>

@property (nonatomic, copy) NSString * partID;
@property (nonatomic, nonatomic) unsigned int size;
@property (nonatomic, nonatomic) MCOEncoding encoding;

- (unsigned int) decodedSize;

@end

#endif
