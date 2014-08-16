//
//  MCONNTPFetchArticlesOperation.h
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCONNTPFETCHMESSAGESOPERATION_H

#define MAILCORE_MCONNTPFETCHMESSAGESOPERATION_H

#import <Foundation/Foundation.h>
#import <MailCore/MCONNTPOperation.h>

/** This is an asynchronous operation that will fetch the list of a messages on the NNTP3 account. */

@interface MCONNTPFetchArticlesOperation : MCONNTPOperation

/** 
 Starts the asynchronous fetch operation.
 
 @param completionBlock Called when the operation is finished.
 
 - On success `error` will be nil and `messages` will be an array of MCONNTPArticleInfo
 
 - On failure, `error` will be set with `MCOErrorDomain` as domain and an 
 error code available in MCOConstants.h, `messages` will be null
 */
- (void) start:(void (^)(NSError * error, NSArray * /* MCONNTPArticleInfo */ messages))completionBlock;

@end

#endif
