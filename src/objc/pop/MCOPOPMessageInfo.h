//
//  MCOPOPMessageInfo.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/30/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MCOPOPMessageInfo : NSObject <NSCopying>

@property (nonatomic, assign) unsigned int index;
@property (nonatomic, assign) unsigned int size;
@property (nonatomic, copy) NSString * uid;

@end
