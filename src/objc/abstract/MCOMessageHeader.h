//
//  MCOMessageHeader.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MCOAddress;

@interface MCOMessageHeader : NSObject

@property (nonatomic, copy) NSString * messageID;
@property (nonatomic, copy) NSArray * references;
@property (nonatomic, copy) NSArray * inReplyTo;
@property (nonatomic, retain) NSDate * date;
@property (nonatomic, retain) NSDate * receivedDate;
@property (nonatomic, copy) MCOAddress * sender;
@property (nonatomic, copy) MCOAddress * from;
@property (nonatomic, copy) NSArray * to;
@property (nonatomic, copy) NSArray * cc;
@property (nonatomic, copy) NSArray * bcc;
@property (nonatomic, copy) NSArray * replyTo;
@property (nonatomic, copy) NSString * subject;
@property (nonatomic, copy) NSString * userAgent;

- (NSString *) extractedSuject;
- (NSString *) partialExtractedSuject;

- (void) importHeadersData:(NSData *)data;

@end
