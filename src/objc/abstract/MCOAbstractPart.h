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

@property (nonatomic, assign) MCOPartType partType;
@property (nonatomic, copy) NSString * filename;
@property (nonatomic, copy) NSString * mimeType;
@property (nonatomic, copy) NSString * charset;
@property (nonatomic, copy) NSString * uniqueID;
@property (nonatomic, copy) NSString * contentID;
@property (nonatomic, copy) NSString * contentLocation;
@property (nonatomic, assign, getter=isInlineAttachment) BOOL inlineAttachment;
@property (nonatomic, weak) MCOAbstractMessage * message;

- (MCOAbstractPart *) partForContentID:(NSString *)contentID;
- (MCOAbstractPart *) partForUniqueID:(NSString *)uniqueID;

@end

#endif
