//
//  MCOMessageParser.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOMESSAGEPARSER_H_

#define __MAILCORE_MCOMESSAGEPARSER_H_

#import <mailcore/MCOAbstractMessage.h>

@protocol MCOMessageParserDelegate;

@interface MCOMessageParser : MCOAbstractMessage

@property (nonatomic, assign) id <MCOMessageParserDelegate> delegate;

+ (MCOMessageParser *) messageParserWithData:(NSData *)data;

- (id) initWithData:(NSData *)data;
- (void) dealloc;

- (MCOAbstractPart *) mainPart;
- (NSData *) data;

- (NSString *) htmlRendering;

@end

@protocol MCOMessageParserDelegate

- (BOOL) MCOMessageParser:(MCOMessageParser *)parser canPreviewPart:(MCOAbstractPart *)part;
- (NSDictionary *) MCOMessageParser:(MCOMessageParser *)parser templateValuesForHeader:(MCOMessageHeader *)part;
- (NSDictionary *) MCOMessageParser:(MCOMessageParser *)parser templateValuesForPart:(MCOAbstractPart *)part;
- (NSString *) MCOMessageParser_templateForMainHeader:(MCOMessageParser *)parser;
- (NSString *) MCOMessageParser_templateForImage:(MCOMessageParser *)parser;
- (NSString *) MCOMessageParser_templateForAttachment:(MCOMessageParser *)parser;
- (NSString *) MCOMessageParser_templateForMessage:(MCOMessageParser *)parser;
- (NSString *) MCOMessageParser_templateForEmbeddedMessage:(MCOMessageParser *)parser;
- (NSString *) MCOMessageParser_templateForEmbeddedMessageHeader:(MCOMessageParser *)parser;
- (NSString *) MCOMessageParser_templateForAttachmentSeparator:(MCOMessageParser *)parser;

- (NSString *) MCOMessageParser_filterHTMLForPart:(NSString *)html;
- (NSString *) MCOMessageParser_filterHTMLForMessage:(NSString *)html;

@end

#endif
