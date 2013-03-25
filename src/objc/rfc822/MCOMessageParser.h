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

@protocol MCOHTMLRendererDelegate;

@interface MCOMessageParser : MCOAbstractMessage <NSCopying>

+ (MCOMessageParser *) messageParserWithData:(NSData *)data;

- (id) initWithData:(NSData *)data;
- (void) dealloc;

- (MCOAbstractPart *) mainPart;
- (NSData *) data;

- (NSString *) htmlRenderingWithDelegate:(id <MCOHTMLRendererDelegate>)delegate;

@end

#endif
