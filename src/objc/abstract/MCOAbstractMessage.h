//
//  MCOAbstractMessage.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCOABSTRACTMESSAGE_H

#define MAILCORE_MCOABSTRACTMESSAGE_H

#import <Foundation/Foundation.h>

@class MCOMessageHeader;
@class MCOAbstractPart;

#if __has_feature(objc_generics)
#  define MCO_GENERIC(...)  <__VA_ARGS__>
#  define MCO_GENERIC_KINDOF(...) <__kindof __VA_ARGS__>
#else
#  define MCO_GENERIC(...)
#  define MCO_GENERIC_KINDOF(...)
#endif

#ifdef __cplusplus
namespace mailcore {
    class AbstractMessage;
}
#endif

@interface MCOAbstractMessage : NSObject <NSCopying>

#ifdef __cplusplus
- (instancetype) initWithMCMessage:(mailcore::AbstractMessage *)message NS_DESIGNATED_INITIALIZER;
#endif

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
- (NSArray MCO_GENERIC_KINDOF(MCOAbstractPart *) *) attachments;

/** All image attachments included inline in the message through cid: URLs.
 It will return an array of MCOIMAPPart for MCOIMAPMessage.
 It will return an array of MCOAttachment for MCOMessageParser.
 It will return an array of MCOAttachment for MCOMessageBuilder. */
- (NSArray MCO_GENERIC_KINDOF(MCOAbstractPart *) *) htmlInlineAttachments;

/**
 Returns parts required to render the message as plain text or html.
 This does not include inline images and attachments, but only the text content
 */
- (NSArray MCO_GENERIC_KINDOF(MCOAbstractPart *) *) requiredPartsForRendering;

@end

#endif
