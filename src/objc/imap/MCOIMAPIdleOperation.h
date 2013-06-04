//
//  MCOIMAPIdleOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPIDLEOPERATION_H_

#define __MAILCORE_MCOIMAPIDLEOPERATION_H_

/** 
 This class implements an IMAP IDLE. IDLE is used to keep a connection
 open with the server so that new messages can be pushed to the client.
 See [RFC2177](http://tools.ietf.org/html/rfc2177)
*/

#import <MailCore/MCOIMAPBaseOperation.h>

@interface MCOIMAPIdleOperation : MCOIMAPBaseOperation

/** Stop the current IDLE session */
- (void) interruptIdle;

/** 
 Starts IDLE

 @param completionBlock Called when the IDLE times out, errors out or detects a change

 - On success `error` will be nil
 
 - On failure, `error` will be set with `MCOErrorDomain` as domain and an 
   error code available in `MCOConstants.h`
*/
- (void) start:(void (^)(NSError * error))completionBlock;

@end

#endif
