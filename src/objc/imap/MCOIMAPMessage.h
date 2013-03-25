//
//  MCOIMAPMessage.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <mailcore/MCOAbstractMessage.h>
#import <mailcore/MCOConstants.h>

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
