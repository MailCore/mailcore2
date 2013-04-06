//
//  MCOIMAPFetchFoldersOperation.h
//  mailcore2
//
//  Created by Matt Ronge on 1/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPFETCHFOLDERSOPERATION_H_

#define __MAILCORE_MCOIMAPFETCHFOLDERSOPERATION_H_

// This class implements an operation to fetch a list of folders.

#import <MailCore/MCOIMAPBaseOperation.h>

@interface MCOIMAPFetchFoldersOperation : MCOIMAPBaseOperation

// Starts the asynchronous operation.
// On success, the completion block will be called with nil as error. folders will be an array of MCOIMAPFolder.
// On failure, error will be set with MCOErrorDomain as domain and an error code available in MCOConstants.h.
// folders will be nil.
- (void) start:(void (^)(NSError * error, NSArray * /* MCOIMAPFolder */ folders))completionBlock;

@end

#endif
