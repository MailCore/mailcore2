//
//  MCOIMAPPart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOAbstractPart.h>

#import <mailcore/MCOConstants.h>

@interface MCOIMAPPart : MCOAbstractPart <NSCopying>

@property (nonatomic, copy) NSString * partID;
@property (nonatomic, nonatomic) unsigned int size;
@property (nonatomic, nonatomic) MCOEncoding encoding;

- (unsigned int) decodedSize;

@end
