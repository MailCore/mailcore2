//
//  MCOAttachment.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOATTACHMENT_H_

#define __MAILCORE_MCOATTACHMENT_H_

#import <mailcore/MCOAbstractPart.h>

@interface MCOAttachment : MCOAbstractPart <NSCopying>

+ (NSString *) mimeTypeForFilename:(NSString *)filename;
+ (MCOAttachment *) attachmentWithContentsOfFile:(NSString *)filename;
+ (MCOAttachment *) attachmentWithHTMLString:(NSString *)htmlString;
+ (MCOAttachment *) attachmentWithRFC822Message:(NSData *)messageData;
+ (MCOAttachment *) attachmentWithText:(NSString *)text;

@property (nonatomic, strong) NSData * data;
 
@end

#endif
