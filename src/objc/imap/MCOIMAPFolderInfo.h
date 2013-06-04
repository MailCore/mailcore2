//
//  MCOIMAPFolderInfo.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPFOLDERINFO_H_

#define __MAILCORE_MCOIMAPFOLDERINFO_H_

#import <Foundation/Foundation.h>

// This class is a cluster for some folder information.

@interface MCOIMAPFolderInfo : NSObject

// Returns a new allocated folder info.
+ (MCOIMAPFolderInfo *) info;

// UIDNEXT value.
@property (nonatomic, assign) uint32_t uidNext;

// UIDVALIDITY value.
@property (nonatomic, assign) uint32_t uidValidity;

// MODSEQ value.
@property (nonatomic, assign) uint64_t modSequenceValue;

// number of messages.
@property (nonatomic, assign) int messageCount;

// first uid of the unseen messages.
@property (nonatomic, assign) uint32_t firstUnseenUid;

@end

#endif
