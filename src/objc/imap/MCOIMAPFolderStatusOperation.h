//
//  MCOIMAPFolderStatusOperation.h
//  mailcore2
//
//  Created by Sebastian on 6/5/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCOIMAPFOLDERSTATUSOPERATION_H

#define MAILCORE_MCOIMAPFOLDERSTATUSOPERATION_H

#import <MailCore/MCOIMAPBaseOperation.h>

/**
 The class is used to get folder metadata (like UIDVALIDITY, UIDNEXT, etc).
 @see MCOIMAPFolderStatus
 */

@class MCOIMAPFolderStatus;

@interface MCOIMAPFolderStatusOperation : MCOIMAPBaseOperation

/**
 Starts the asynchronous operation.
 
 @param completionBlock Called when the operation is finished.
 
 - On success `error` will be nil and `status` will contain the status metadata
 
 - On failure, `error` will be set with `MCOErrorDomain` as domain and an
 error code available in `MCOConstants.h`, `info` will be nil
 */

- (void) start:(void (^)(NSError * error, MCOIMAPFolderStatus * status))completionBlock;

@end

#endif
