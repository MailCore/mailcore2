//
//  MCOIMAPPart.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPABSTRACTPART_H_

#define __MAILCORE_MCOIMAPABSTRACTPART_H_

#import <MailCore/MCOAbstractPart.h>

#import <MailCore/MCOConstants.h>

/** Represents a single IMAP message part */

@interface MCOIMAPPart : MCOAbstractPart

/** A part identifier looks like 1.2.1 */
@property (nonatomic, copy) NSString * partID;

/** The size of the single part in bytes */
@property (nonatomic, nonatomic) unsigned int size;

/** It's the encoding of the single part */
@property (nonatomic, nonatomic) MCOEncoding encoding;

/**
 Returns the decoded size of the part.
 For example, for a part that's encoded with base64, it will return actual_size * 3/4.
*/
- (unsigned int) decodedSize;

@end

#endif
