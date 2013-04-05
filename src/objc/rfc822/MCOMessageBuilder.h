//
//  MCOMessageBuilder.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOMESSAGEBUILDER_H_

#define __MAILCORE_MCOMESSAGEBUILDER_H_

#import <MailCore/MCOAbstractMessage.h>

// This class will allow you to build a RFC 822 formatted message.

@class MCOAttachment;
@protocol MCOHTMLRendererDelegate;

@interface MCOMessageBuilder : MCOAbstractMessage <NSCopying>

// Main HTML content of the message.
@property (nonatomic, copy, setter=setHTMLBody:) NSString * htmlBody;

// Plain text content of the message.
@property (nonatomic, copy) NSString * textBody;

// List of file attachments.
@property (nonatomic, copy) NSArray * attachments;

// List of related file attachments (included as cid: link in the HTML part).
@property (nonatomic, copy) NSArray * relatedAttachments;

// Prefix for the boundary identifier
@property (nonatomic, copy) NSString * boundaryPrefix;

// Add an attachment.
- (void) addAttachment:(MCOAttachment *)attachment;

// Add a related attachment.
- (void) addRelatedAttachment:(MCOAttachment *)attachment;

// RFC 822 formatted message.
- (NSData *) data;

// HTML rendering of the message to be displayed in a web view.
- (NSString *) htmlRenderingWithDelegate:(id <MCOHTMLRendererDelegate>)delegate;

@end

#endif
