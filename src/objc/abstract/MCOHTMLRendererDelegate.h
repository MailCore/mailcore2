//
//  MCOHTMLRendererDelegate.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef mailcore2_MCOHTMLRendererDelegate_h
#define mailcore2_MCOHTMLRendererDelegate_h

@protocol MCOHTMLRendererDelegate <NSObject>

- (BOOL) MCOAbstractMessage:(MCOAbstractMessage *)msg canPreviewPart:(MCOAbstractPart *)part;
- (NSDictionary *) MCOAbstractMessage:(MCOAbstractMessage *)msg templateValuesForHeader:(MCOMessageHeader *)part;
- (NSDictionary *) MCOAbstractMessage:(MCOAbstractMessage *)msg templateValuesForPart:(MCOAbstractPart *)part;
- (NSString *) MCOAbstractMessage_templateForMainHeader:(MCOAbstractMessage *)msg;
- (NSString *) MCOAbstractMessage_templateForImage:(MCOAbstractMessage *)msg;
- (NSString *) MCOAbstractMessage_templateForAttachment:(MCOAbstractMessage *)msg;
- (NSString *) MCOAbstractMessage_templateForMessage:(MCOAbstractMessage *)msg;
- (NSString *) MCOAbstractMessage_templateForEmbeddedMessage:(MCOAbstractMessage *)msg;
- (NSString *) MCOAbstractMessage_templateForEmbeddedMessageHeader:(MCOAbstractMessage *)msg;
- (NSString *) MCOAbstractMessage_templateForAttachmentSeparator:(MCOAbstractMessage *)msg;

- (NSString *) MCOAbstractMessage:(MCOAbstractMessage *)msg filterHTMLForPart:(NSString *)html;
- (NSString *) MCOAbstractMessage:(MCOAbstractMessage *)msg filterHTMLForMessage:(NSString *)html;

@end

#endif
