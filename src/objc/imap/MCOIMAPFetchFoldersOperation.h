//
//  MCOIMAPFetchFoldersOperation.h
//  mailcore2
//
//  Created by Matt Ronge on 1/31/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCOIMAPFETCHFOLDERSOPERATION_H

#define MAILCORE_MCOIMAPFETCHFOLDERSOPERATION_H

/** This class implements an operation to fetch a list of folders. */

#import <MailCore/MCOIMAPBaseOperation.h>

@interface MCOIMAPFetchFoldersOperation : MCOIMAPBaseOperation

/** 
 Starts the asynchronous fetch operation.

 @param completionBlock Called when the operation is finished.

 - On success `error` will be nil and `folders` will contain an array of MCOIMAPFolder
 
 - On failure, `error` will be set with `MCOErrorDomain` as domain and an 
   error code available in `MCOConstants.h`, `folders` will be nil
*/
- (void) start:(void (^)(NSError * error, NSArray * /* MCOIMAPFolder */ folders))completionBlock;

@end

#endif
