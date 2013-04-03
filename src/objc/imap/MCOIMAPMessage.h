//
//  MCOIMAPMessage.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <MailCore/MCOAbstractMessage.h>
#import <MailCore/MCOConstants.h>

#ifndef __MAILCORE_MCOIMAPMESSAGE_H_

#define __MAILCORE_MCOIMAPMESSAGE_H_

@protocol MCOHTMLRendererIMAPDelegate;

@interface MCOIMAPMessage : MCOAbstractMessage <NSCopying>

@property (nonatomic, assign) uint32_t uid;
@property (nonatomic, assign) MCOMessageFlag flags;
@property (nonatomic, assign) MCOMessageFlag originalFlags;
@property (nonatomic, assign) uint64_t modSeqValue;
@property (nonatomic, retain) MCOAbstractPart * mainPart;
@property (nonatomic, copy) NSArray * gmailLabels;

- (MCOAbstractPart *) partForPartID:(NSString *)partID;
- (MCOAbstractPart *) partForContentID:(NSString *)contentID;
- (MCOAbstractPart *) partForUniqueID:(NSString *)uniqueID;

- (NSString *) htmlRenderingWithFolder:(NSString *)folder
                              delegate:(id <MCOHTMLRendererIMAPDelegate>)delegate;

@end

#endif
