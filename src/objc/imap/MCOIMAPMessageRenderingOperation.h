//
//  MCOIMAPMessageRenderingOperation.h
//  mailcore2
//
//  Created by Paul Young on 07/07/2013.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPMESSAGERENDERINGOPERATION_H_

#define __MAILCORE_MCOIMAPMESSAGERENDERINGOPERATION_H_

#import <MailCore/MCOIMAPBaseOperation.h>

/**
 The class is used to get the HTML string of a message.
 */

@interface MCOIMAPMessageRenderingOperation : MCOIMAPBaseOperation

/**
 Starts the asynchronous operation.
 
 @param completionBlock Called when the operation is finished.
 
 - On success `error` will be nil and `htmlString` will contain the message
 
 - On failure, `error` will be set and `htmlString` will be nil
 */

- (void) start:(void (^)(NSString * htmlString, NSError * error))completionBlock;

@end

#endif
