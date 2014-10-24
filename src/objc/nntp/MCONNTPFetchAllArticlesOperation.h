//
//  MCONNTPFetchAllArticlesOperation.h
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCONNTPFETCHARTICLESOPERATION_H

#define MAILCORE_MCONNTPFETCHARTICLESOPERATION_H

#import <Foundation/Foundation.h>
#import <MailCore/MCONNTPOperation.h>

@class MCOIndexSet;

/** This is an asynchronous operation that will fetch the list of a messages on the NNTP server. */
@interface MCONNTPFetchAllArticlesOperation : MCONNTPOperation

/** 
 Starts the asynchronous fetch operation.
 
 @param completionBlock Called when the operation is finished.
 
 - On success `error` will be nil and `articles` will be an index set of article numbers.
 
 - On failure, `error` will be set with `MCOErrorDomain` as domain and an 
 error code available in MCOConstants.h, `messages` will be null
 */
- (void) start:(void (^)(NSError * error, MCOIndexSet * articles))completionBlock;

@end

#endif
