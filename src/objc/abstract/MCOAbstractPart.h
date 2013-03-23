//
//  MCOAbstractPart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

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
@property (nonatomic, assign) MCOAbstractMessage * message;

- (MCOAbstractPart *) partForContentID:(NSString *)contentID;
- (MCOAbstractPart *) partForUniqueID:(NSString *)uniqueID;

@end
