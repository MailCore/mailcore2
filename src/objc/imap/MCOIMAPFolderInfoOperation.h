//
//  MCOIMAPFolderInfoOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPFOLDERINFOOPERATION_H_

#define __MAILCORE_MCOIMAPFOLDERINFOOPERATION_H_

#import <MailCore/MCOIMAPBaseOperation.h>

// This class implements the operation that will fetch some folder information
// (uidNext uidValidity, modSequenceValue, messageCount).

@class MCOIMAPFolderInfo;

@interface MCOIMAPFolderInfoOperation : MCOIMAPBaseOperation

// Starts the asynchronous operation.
// On success, the completion block will be called with nil as error. info contains the folder information.
// On failure, error will be set with MCOErrorDomain as domain and an error code available in MCOConstants.h.
// info will be nil.
- (void) start:(void (^)(NSError * error, MCOIMAPFolderInfo *info))completionBlock;

@end

#endif
