//
//  MCOIMAPMessage.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPMESSAGE_H_

#define __MAILCORE_MCOIMAPMESSAGE_H_

/** 
 This represents an IMAP message.

 If you fetched the MIME structure of the message, you can fetch
 efficiently the content of the message by fetching only the parts
 that you need to show it.

 For example, you could fetch only the text parts to show the summary
 of the message, using [MCOIMAPSession fetchMessageAttachmentByUIDOperationWithFolder:uid:partID:encoding:]

 You can also decide to fetch entirely the message using
 [MCOIMAPSession fetchMessageByUIDOperationWithFolder:uid:]
*/

#import <MailCore/MCOAbstractMessage.h>
#import <MailCore/MCOConstants.h>

@protocol MCOHTMLRendererIMAPDelegate;

@interface MCOIMAPMessage : MCOAbstractMessage <NSCopying>

/** IMAP UID of the message. */
@property (nonatomic, assign) uint32_t uid;

/** Flags of the message, like if it is deleted, read, starred etc */
@property (nonatomic, assign) MCOMessageFlag flags;

/** The contents of the message flags when it was fetched from the server */
@property (nonatomic, assign) MCOMessageFlag originalFlags;

/** It's the last modification sequence value of the message synced from the server. See RFC4551 */
@property (nonatomic, assign) uint64_t modSeqValue;

/** Main MIME part of the message */
@property (nonatomic, retain) MCOAbstractPart * mainPart;

/** All Gmail labels of the message */
@property (nonatomic, copy) NSArray * /* NSString */ gmailLabels;

/** Gmail thread iD of the message */
@property (nonatomic, assign) uint64_t gmailThreadID;

/**
 Returns the part with the given part identifier.
 @param partID A part identifier looks like 1.2.1
*/
- (MCOAbstractPart *) partForPartID:(NSString *)partID;

/**
 HTML rendering of the message to be displayed in a web view.
 The delegate should implement at least
 [MCOAbstractMessage:dataForIMAPPart:folder:]
 so that the complete HTML rendering can take place.
*/
- (NSString *) htmlRenderingWithFolder:(NSString *)folder
                              delegate:(id <MCOHTMLRendererIMAPDelegate>)delegate;

@end

#endif
