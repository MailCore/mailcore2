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

/** This class implements common fields of a message header.*/

@class MCOAddress;

@interface MCOMessageHeader : NSObject <NSCopying>

/** Message-ID field.*/
@property (nonatomic, copy) NSString * messageID;

/** References field. It's an array of message-ids.*/
@property (nonatomic, copy) NSArray * /* NSString */ references;

/** In-Reply-To field. It's an array of message-ids.*/
@property (nonatomic, copy) NSArray * /* NSString */ inReplyTo;

/** Date field: sent date of the message.*/
@property (nonatomic, strong) NSDate * date;

/** Received date: received date of the message.*/
@property (nonatomic, strong) NSDate * receivedDate;

/** Sender field.*/
@property (nonatomic, copy) MCOAddress * sender;

/** From field: address of the sender of the message.*/
@property (nonatomic, copy) MCOAddress * from;

/** To field: recipient of the message. It's an array of MCOAddress.*/
@property (nonatomic, copy) NSArray * /* MCOAddress */ to;

/** Cc field: cc recipient of the message. It's an array of MCOAddress.*/
@property (nonatomic, copy) NSArray * /* MCOAddress */ cc;

/** Bcc field: bcc recipient of the message. It's an array of MCOAddress.*/
@property (nonatomic, copy) NSArray * /* MCOAddress */ bcc;

/** Reply-To field. It's an array of MCOAddress.*/
@property (nonatomic, copy) NSArray * /* MCOAddress */ replyTo;

/** Subject of the message.*/
@property (nonatomic, copy) NSString * subject;

/** User-Agent.*/
@property (nonatomic, copy) NSString * userAgent;

/** Adds a custom header.*/
- (void)addHeaderValue:(NSString *)value forName:(NSString *)name;

/** Remove a given custom header.*/
- (void)removeHeaderForName:(NSString *)name;

/** Returns the value of a given custom header.*/
- (NSString *)headerValueForName:(NSString *)name;

/** Returns an array with the names of all custom headers.*/
- (NSArray * /* NSString */)allHeadersNames;

/** Extracted subject (also remove square brackets).*/
- (NSString *) extractedSubject;

/** Extracted subject (don't remove square brackets).*/
- (NSString *) partialExtractedSubject;

/** Fill the header using the given RFC 822 data.*/
- (void) importHeadersData:(NSData *)data;

/** Returns a header that can be used as a base for a reply message.*/
- (MCOMessageHeader *) replyHeaderWithExcludedRecipients:(NSArray *)excludedRecipients;

/** Returns a header that can be used as a base for a reply all message.*/
- (MCOMessageHeader *) replyAllHeaderWithExcludedRecipients:(NSArray *)excludedRecipients;

/** Returns a header that can be used as a base for a forward message.*/
- (MCOMessageHeader *) forwardHeader;

@end

#endif
