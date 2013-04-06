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
    MCOPartTypeSingle,
    MCOPartTypeMessage,
    MCOPartTypeMultipartMixed,
    MCOPartTypeMultipartRelated,
    MCOPartTypeMultipartAlternative,
} MCOPartType;

@interface MCOAbstractPart : NSObject

// Returns type of the part (single / message part / multipart/mixed,
// multipart/related, multipart/alternative). See MCOPartType.
@property (nonatomic, assign) MCOPartType partType;

// Returns filename of the part.
@property (nonatomic, copy) NSString * filename;

// Returns MIME type of the part. For example application/data.
@property (nonatomic, copy) NSString * mimeType;

// Returns charset of the part in case it's a text single part.
@property (nonatomic, copy) NSString * charset;

// Returns the unique ID generated for this part.
// It's a unique identifier that's created when the object is created manually
// or created by the parser.
@property (nonatomic, copy) NSString * uniqueID;

// Returns the value of the Content-ID field of the part.
@property (nonatomic, copy) NSString * contentID;

// Returns the value of the Content-Location field of the part.
@property (nonatomic, copy) NSString * contentLocation;

// Returns whether the part is an explicit inline attachment.
@property (nonatomic, assign, getter=isInlineAttachment) BOOL inlineAttachment;

// Returns the owner message of the part.
@property (nonatomic, weak) MCOAbstractMessage * message;

// Returns the part with the given Content-ID among this part and its subparts.
- (MCOAbstractPart *) partForContentID:(NSString *)contentID;

// Returns the part with the given unique identifier among this part and its subparts.
- (MCOAbstractPart *) partForUniqueID:(NSString *)uniqueID;

@end

#endif
