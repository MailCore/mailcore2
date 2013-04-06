//
//  MCOHTMLRendererDelegate.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOHTMLRENDERERDELEGATE_H_

#define __MAILCORE_MCOHTMLRENDERERDELEGATE_H_

#import <Foundation/Foundation.h>

// This delegate protocol is used to help rendering of the message.
//
// It will be used for the following methods.
//
// -[MCOMessageParser htmlRenderingWithDelegate:],
// -[MCOMessageBuilder htmlRenderingWithDelegate:]
// -[MCOIMAPMessage htmlRenderingWithFolder:delegate:]

@class MCOAbstractPart;
@class MCOAbstractMessage;
@class MCOMessageHeader;

@protocol MCOHTMLRendererDelegate <NSObject>

// All methods are optional.
@optional

// This delegate method should return YES if it can render a preview of the attachment as an image.
// part is always a single part.
// If the attachment can be previewed, it will be rendered using the image template.
// If not, the attachment template will be used.
- (BOOL) MCOAbstractMessage:(MCOAbstractMessage *)msg canPreviewPart:(MCOAbstractPart *)part;

// This delegate method returns the values to be applied to the template for the given header.
// See the content of MCHTMLRendererCallback.cpp for the default values of the header.
- (NSDictionary *) MCOAbstractMessage:(MCOAbstractMessage *)msg templateValuesForHeader:(MCOMessageHeader *)header;

// This delegate method returns the values to be applied to the template for the given attachment.
// See the content of MCHTMLRendererCallback.cpp for the default values of the attachment.
- (NSDictionary *) MCOAbstractMessage:(MCOAbstractMessage *)msg templateValuesForPart:(MCOAbstractPart *)part;

// The following methods returns templates. They will match the syntax of ctemplate.
// See https://code.google.com/p/ctemplate/

// This delegate method returns the template for the main header of the message.
// See the content of MCHTMLRendererCallback.cpp for the default values of the template.
- (NSString *) MCOAbstractMessage_templateForMainHeader:(MCOAbstractMessage *)msg;

// This delegate method returns the template an image attachment.
- (NSString *) MCOAbstractMessage_templateForImage:(MCOAbstractMessage *)msg;

// This delegate method returns the template attachment other than images.
// See the content of MCHTMLRendererCallback.cpp for the default values of the template.
- (NSString *) MCOAbstractMessage_templateForAttachment:(MCOAbstractMessage *)msg;

// This delegate method returns the template of the main message.
// It should include HEADER and a BODY values.
// See the content of MCHTMLRendererCallback.cpp for the default values of the template.
- (NSString *) MCOAbstractMessage_templateForMessage:(MCOAbstractMessage *)msg;

// This delegate method returns the template of an embedded message (included as attachment).
// It should include HEADER and a BODY values.
// See the content of MCHTMLRendererCallback.cpp for the default values of the template.
- (NSString *) MCOAbstractMessage_templateForEmbeddedMessage:(MCOAbstractMessage *)msg;

// This delegate method returns the template for the header of an embedded message.
// See the content of MCHTMLRendererCallback.cpp for the default values of the template.
- (NSString *) MCOAbstractMessage_templateForEmbeddedMessageHeader:(MCOAbstractMessage *)msg;

// This delegate method returns the separator between the text of the message and the attachments.
// This delegate method returns the template for the header of an embedded message.
- (NSString *) MCOAbstractMessage_templateForAttachmentSeparator:(MCOAbstractMessage *)msg;

// The following methods will filter the HTML content and may apply some filters to
// change how to display the message.

// This delegate method will apply the filter to HTML rendered content of a given text part.
// For example, it could filter the CSS content.
- (NSString *) MCOAbstractMessage:(MCOAbstractMessage *)msg filterHTMLForPart:(NSString *)html;

// This delegate method will apply a filter to the whole HTML content.
// For example, it could collapse the quoted messages.
- (NSString *) MCOAbstractMessage:(MCOAbstractMessage *)msg filterHTMLForMessage:(NSString *)html;

@end

#endif
