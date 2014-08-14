//
//  MCONNTPMessageInfo.h
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCONNTPMESSAGEINFO_H

#define MAILCORE_MCONNTPMESSAGEINFO_H

#import <Foundation/Foundation.h>

/** This is information of a message fetched by MCONNTPFetchArticlesOperation.*/

@interface MCONNTPMessageInfo : NSObject <NSCopying>

/** This is the index of a given message.*/
@property (nonatomic, assign) unsigned int index;

/** This is the size of the given message.*/
@property (nonatomic, assign) unsigned int size;

/** This is the unique identifier of the message.
 It can be used as a cache identifier.*/
@property (nonatomic, copy) NSString * uid;

@end

#endif
