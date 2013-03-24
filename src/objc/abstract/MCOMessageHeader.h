//
//  MCOMessageHeader.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOMESSAGEHEADER_H_

#define __MAILCORE_MCOMESSAGEHEADER_H_

#import <Foundation/Foundation.h>

@class MCOAddress;

@interface MCOMessageHeader : NSObject <NSCopying>

@property (nonatomic, copy) NSString * messageID;
@property (nonatomic, copy) NSArray * references;
@property (nonatomic, copy) NSArray * inReplyTo;
@property (nonatomic, strong) NSDate * date;
@property (nonatomic, strong) NSDate * receivedDate;
@property (nonatomic, copy) MCOAddress * sender;
@property (nonatomic, copy) MCOAddress * from;
@property (nonatomic, copy) NSArray * to;
@property (nonatomic, copy) NSArray * cc;
@property (nonatomic, copy) NSArray * bcc;
@property (nonatomic, copy) NSArray * replyTo;
@property (nonatomic, copy) NSString * subject;
@property (nonatomic, copy) NSString * userAgent;

- (NSString *) extractedSubject;
- (NSString *) partialExtractedSubject;

- (void) importHeadersData:(NSData *)data;

@end

#endif
