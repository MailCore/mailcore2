//
//  MCOPOPMessageInfo.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/30/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOPOPMESSAGEINFO_H_

#define __MAILCORE_MCOPOPMESSAGEINFO_H_

#import <Foundation/Foundation.h>

/** This is information of a message fetched by MCOPOPFetchMessagesOperation.*/

@interface MCOPOPMessageInfo : NSObject <NSCopying>

/** This is the index of a given message.*/
@property (nonatomic, assign) unsigned int index;

/** This is the size of the given message.*/
@property (nonatomic, assign) unsigned int size;

/** This is the unique identifier of the message.
 It can be used as a cache identifier.*/
@property (nonatomic, copy) NSString * uid;

@end

#endif
