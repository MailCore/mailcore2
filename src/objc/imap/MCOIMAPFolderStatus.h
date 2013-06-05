//
//  MCOIMAPFolderStatus.h
//  mailcore2
//
//  Created by Sebastian on 6/5/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPFOLDERSTATUS_H_

#define __MAILCORE_MCOIMAPFOLDERSTATUS_H_

#import <Foundation/Foundation.h>

/* This class holds IMAP folder metadata */

@interface MCOIMAPFolderStatus : NSObject

/** Create an empty instance */
+ (MCOIMAPFolderStatus *) status;

/** The folder's IMAP UIDNEXT value. Used to determine the uid for the next received message. */
@property (nonatomic, assign) uint32_t uidNext;

/** The folders IMAP UIDVALIDITY value. Must be used to determine if the server has changed assigned UIDs */
@property (nonatomic, assign) uint32_t uidValidity;

/** Number of recent messages received in the folder */
@property (nonatomic, assign) uint32_t totalRecent;

/** Number of unseen messages in the folder */
@property (nonatomic, assign) uint32_t totalUnseen;

// Number of messages in the folder
@property (nonatomic, assign) uint32_t totalMessages;

// We need also the the folder path



@end

#endif
