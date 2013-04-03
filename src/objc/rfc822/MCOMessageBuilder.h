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

@class MCOAttachment;
@protocol MCOHTMLRendererDelegate;

@interface MCOMessageBuilder : MCOAbstractMessage <NSCopying>

@property (nonatomic, copy, setter=setHTMLBody:) NSString * htmlBody;
@property (nonatomic, copy) NSString * textBody;
@property (nonatomic, copy) NSArray * attachments;
@property (nonatomic, copy) NSArray * relatedAttachments;
@property (nonatomic, copy) NSString * boundaryPrefix;

- (void) addAttachment:(MCOAttachment *)attachment;
- (void) addRelatedAttachment:(MCOAttachment *)attachment;

- (NSData *) data;

- (NSString *) htmlRenderingWithDelegate:(id <MCOHTMLRendererDelegate>)delegate;

@end

#endif
