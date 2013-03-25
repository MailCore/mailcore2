//
//  MCOIMAPMessagePart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOAbstractMessagePart.h>

@interface MCOIMAPMessagePart : MCOAbstractMessagePart <NSCopying>

@property (nonatomic, copy) NSString * partID;

@end
