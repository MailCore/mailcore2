//
//  MCOIMAPMessage.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPMESSAGE_H_

#define __MAILCORE_MCOIMAPMESSAGE_H_

// This class implements an IMAP message.

#import <MailCore/MCOAbstractMessage.h>
#import <MailCore/MCOConstants.h>

@protocol MCOHTMLRendererIMAPDelegate;

@interface MCOIMAPMessage : MCOAbstractMessage <NSCopying>

// It's the UID of the message.
@property (nonatomic, assign) uint32_t uid;

// It's the flags of the message.
@property (nonatomic, assign) MCOMessageFlag flags;

// It's the flags of the message fetched from the server.
@property (nonatomic, assign) MCOMessageFlag originalFlags;

// It's the last modification sequence value of the message synced from the server.
@property (nonatomic, assign) uint64_t modSeqValue;

// It's the main MIME part of the message.
@property (nonatomic, retain) MCOAbstractPart * mainPart;

// It's the Gmail labels of the message.
@property (nonatomic, copy) NSArray * gmailLabels;

// Returns the part with the given part identifier.
// A part identifier looks like 1.2.1
- (MCOAbstractPart *) partForPartID:(NSString *)partID;

// HTML rendering of the message to be displayed in a web view.
// The delegate should implement at least
// -MCOAbstractMessage:dataForIMAPPart:folder:
// so that the complete HTML rendering can have a chance to take place.
- (NSString *) htmlRenderingWithFolder:(NSString *)folder
                              delegate:(id <MCOHTMLRendererIMAPDelegate>)delegate;

@end

#endif
