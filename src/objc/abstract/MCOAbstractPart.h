//
//  MCOAbstractPart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOABSTRACTPART_H_

#define __MAILCORE_MCOABSTRACTPART_H_

#import <Foundation/Foundation.h>

@class MCOAbstractMessage;

typedef enum {
    // Used for a single part.
    // The part will be a MCOAbstractPart.
    MCOPartTypeSingle,
    
    // Used for a message part (MIME type: message/rfc822).
    // The part will be a MCOAbstractMessagePart.
    // It's when a message is sent as attachment of an other message.
    MCOPartTypeMessage,
    
    // Used for a multipart, multipart/mixed.
    // The part will be a MCOAbstractMultipart.
    MCOPartTypeMultipartMixed,
    
    // Used for a multipart, multipart/related.
    // The part will be a MCOAbstractMultipart.
    MCOPartTypeMultipartRelated,
    
    // Used for a multipart, multipart/alternative.
    // The part will be a MCOAbstractMultipart.
    MCOPartTypeMultipartAlternative,
} MCOPartType;

@interface MCOAbstractPart : NSObject <NSCopying>

/** Returns type of the part (single / message part / multipart/mixed,
 multipart/related, multipart/alternative). See MCOPartType.*/
@property (nonatomic, assign) MCOPartType partType;

/** Returns filename of the part.*/
@property (nonatomic, copy) NSString * filename;

/** Returns MIME type of the part. For example application/data.*/
@property (nonatomic, copy) NSString * mimeType;

/** Returns charset of the part in case it's a text single part.*/
@property (nonatomic, copy) NSString * charset;

/** Returns the unique ID generated for this part.
 It's a unique identifier that's created when the object is created manually
 or created by the parser.*/
@property (nonatomic, copy) NSString * uniqueID;

/** Returns the value of the Content-ID field of the part.*/
@property (nonatomic, copy) NSString * contentID;

/** Returns the value of the Content-Location field of the part.*/
@property (nonatomic, copy) NSString * contentLocation;

/** Returns whether the part is an explicit inline attachment.*/
@property (nonatomic, assign, getter=isInlineAttachment) BOOL inlineAttachment;

/** Returns the part with the given Content-ID among this part and its subparts.*/
- (MCOAbstractPart *) partForContentID:(NSString *)contentID;

/** Returns the part with the given unique identifier among this part and its subparts.*/
- (MCOAbstractPart *) partForUniqueID:(NSString *)uniqueID;

/** Returns a string representation of the data according to charset.*/
- (NSString *) decodedStringForData:(NSData *)data;

@end

#endif
