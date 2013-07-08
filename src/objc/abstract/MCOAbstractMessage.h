//
//  MCOAbstractMessage.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOABSTRACTMESSAGE_H_

#define __MAILCORE_MCOABSTRACTMESSAGE_H_

#import <Foundation/Foundation.h>

@class MCOMessageHeader;
@class MCOAbstractPart;

@interface MCOAbstractMessage : NSObject <NSCopying>

/** Header of the message. */
@property (nonatomic, strong) MCOMessageHeader * header;

/** Returns the part with the given Content-ID.*/
- (MCOAbstractPart *) partForContentID:(NSString *)contentID;

/** Returns the part with the given unique identifier.*/
- (MCOAbstractPart *) partForUniqueID:(NSString *)uniqueID;

/** All attachments in the message.
 It will return an array of MCOIMAPPart for MCOIMAPMessage.
 It will return an array of MCOAttachment for MCOMessageParser.
 It will return an array of MCOAttachment for MCOMessageBuilder. */
- (NSArray * /* MCOAbstractPart */) attachments;

/** All image attachments included inline in the message through cid: URLs.
 It will return an array of MCOIMAPPart for MCOIMAPMessage.
 It will return an array of MCOAttachment for MCOMessageParser.
 It will return an array of MCOAttachment for MCOMessageBuilder. */
- (NSArray * /* MCOAbstractPart */) htmlInlineAttachments;

@end

#endif
