//
//  MCOAbstractMultipart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <mailcore/MCOAbstractPart.h>

@interface MCOAbstractMultipart : MCOAbstractPart

@property (nonatomic, copy) NSArray * parts;

@end
