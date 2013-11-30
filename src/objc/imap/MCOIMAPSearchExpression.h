//
//  MCOIMAPSearchExpression.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOIMAPSEARCHEXPRESSION_H_

#define __MAILCORE_MCOIMAPSEARCHEXPRESSION_H_

/** Used to construct an IMAP search query */

#import <Foundation/Foundation.h>
#import <MailCore/MCOConstants.h>

@interface MCOIMAPSearchExpression : NSObject

/** 
 Creates a search expression that returns all UIDS for the mailbox

 Example:
    
    MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchAll]
*/
+ (MCOIMAPSearchExpression *) searchAll;

/**
 Creates a search expression that matches the sender of an email.

 Example:

     MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchFrom:@"laura@etpan.org"]
*/
+ (MCOIMAPSearchExpression *) searchFrom:(NSString *)value;

/**
 Creates a search expression that matches any recipient of an email.

 Example:

    MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchRecipient:@"ngan@etpan.org"]
**/
+ (MCOIMAPSearchExpression *) searchRecipient:(NSString *)value;

/*
 Creates a search expression that matches the subject of an email.

 Example:

     MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchSubject:@"airline"]
**/
+ (MCOIMAPSearchExpression *) searchSubject:(NSString *)value;

/**
 Creates a search expression that matches the content of an email.

 Example:

     MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchContent:@"meeting"]
*/
+ (MCOIMAPSearchExpression *) searchContent:(NSString *)value;

/**
 Creates a search expression that matches the content of a specific header.

 Example:

     MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchHeader:@"List-Id" value:@"shoes"]
**/
+ (MCOIMAPSearchExpression *) searchHeader:(NSString *)header value:(NSString *)value;

/**
 Creates a search expression that matches messages with the Read flag.
 
 Example:
 
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchRead]
 **/
+ (MCOIMAPSearchExpression *) searchRead;

/**
 Creates a search expression that matches messages without the Read flag.
 
 Example:
 
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchUnread]
 **/
+ (MCOIMAPSearchExpression *) searchUnread;

/**
 Creates a search expression that matches messages that have been flagged.
 
 Example:
 
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchFlagged]
 **/
+ (MCOIMAPSearchExpression *) searchFlagged;

/**
 Creates a search expression that matches messages that haven't been flagged.
 
 Example:
 
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchUnflagged]
 **/
+ (MCOIMAPSearchExpression *) searchUnflagged;

/**
 Creates a search expression that matches messages that have the answered flag set.
 
 Example:
 
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchAnswered]
 **/
+ (MCOIMAPSearchExpression *) searchAnswered;

/**
 Creates a search expression that matches messages that don't have the answered flag set..
 
 Example:
 
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchUnanswered]
 **/
+ (MCOIMAPSearchExpression *) searchUnanswered;

/**
 Creates a search expression that matches draft messages.
 
 Example:
 
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchDraft]
 **/
+ (MCOIMAPSearchExpression *) searchDraft;

/**
 Creates a search expression that matches messages that aren't drafts.
 
 Example:
 
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchUndraft]
 **/
+ (MCOIMAPSearchExpression *) searchUndraft;

/**
 Creates a search expression that matches messages that are deleted.
 
 Example:
 
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchDeleted]
 **/
+ (MCOIMAPSearchExpression *) searchDeleted;

/**
 Creates a search expression that matches messages that are spam.
 
 Example:
 
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchSpam]
 **/
+ (MCOIMAPSearchExpression *) searchSpam;

/**
 Creates a search expression that matches messages sent before a date.
 
 Example:
 
 NSDate *yesterday = [NSDate dateWithTimeIntervalSinceNow:-(60.0 * 60.0 * 24.0)];
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchBeforeDate:yesterday]
 **/
+ (MCOIMAPSearchExpression *) searchBeforeDate:(NSDate *)date;

/**
 Creates a search expression that matches messages sent on a date.
 
 Example:
 
 NSDate *now = [NSDate date];
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchOnDate:now]
 **/
+ (MCOIMAPSearchExpression *) searchOnDate:(NSDate *)date;

/**
 Creates a search expression that matches messages sent since a date.
 
 Example:
 
 NSDate *now = [NSDate date];
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchSinceDate:now]
 **/
+ (MCOIMAPSearchExpression *) searchSinceDate:(NSDate *)date;

/**
 Creates a search expression that matches messages received before a date.
 
 Example:
 
 NSDate *yesterday = [NSDate dateWithTimeIntervalSinceNow:-(60.0 * 60.0 * 24.0)];
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchBeforeReceivedDate:yesterday]
 **/
+ (MCOIMAPSearchExpression *) searchBeforeReceivedDate:(NSDate *)date;

/**
 Creates a search expression that matches messages received on a date.
 
 Example:
 
 NSDate *now = [NSDate date];
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchOnReceivedDate:now]
 **/
+ (MCOIMAPSearchExpression *) searchOnReceivedDate:(NSDate *)date;

/**
 Creates a search expression that matches messages received since a date.
 
 Example:
 
 NSDate *yesterday = [NSDate dateWithTimeIntervalSinceNow:-(60.0 * 60.0 * 24.0)];
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchSinceReceivedDate:yesterday]
 **/
+ (MCOIMAPSearchExpression *) searchSinceReceivedDate:(NSDate *)date;

/**
 Creates a search expression that matches messages larger than a given size in bytes.
 
 Example:
 
 uint32_t minSize = 1024 * 10; // 10 KB
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchSizeLargerThan:minSize]
 **/
+ (MCOIMAPSearchExpression *) searchSizeLargerThan:(uint32_t)size;

/**
 Creates a search expression that matches messages smaller than a given size in bytes.
 
 Example:
 
 uint32_t maxSize = 1024 * 10; // 10 KB
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchSizeSmallerThan:maxSize]
 **/
+ (MCOIMAPSearchExpression *) searchSizeSmallerThan:(uint32_t)size;

/**
 Creates a search expression that matches emails with the given gmail thread id
 
 Example:
 
 MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchGmailThreadID:aThreadID]
 */
+ (MCOIMAPSearchExpression *) searchGmailThreadID:(uint64_t)number;

/**
 Creates a search expression that's a disjunction of two search expressions.

 Example:

     MCOIMAPSearchExpression * exprFrom = [MCOIMAPSearchExpression searchFrom:@"laura@etpan.org"]
     MCOIMAPSearchExpression * exprSubject = [MCOIMAPSearchExpression searchContent:@"meeting"]
     MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchAnd:exprFrom other:exprSubject];
**/

+ (MCOIMAPSearchExpression *) searchAnd:(MCOIMAPSearchExpression *)expression other:(MCOIMAPSearchExpression *)other;
/**
 Creates a search expression that's a conjunction of two search expressions.

 Example:

     MCOIMAPSearchExpression * exprFrom = [MCOIMAPSearchExpression searchFrom:@"laura@etpan.org"]
     MCOIMAPSearchExpression * exprOtherFrom = [MCOIMAPSearchExpression searchRecipient:@"ngan@etpan.org"]
     MCOIMAPSearchExpression * expr = [MCOIMAPSearchExpression searchOr:exprFrom exprOtherFrom];
*/
+ (MCOIMAPSearchExpression *) searchOr:(MCOIMAPSearchExpression *)expression other:(MCOIMAPSearchExpression *)other;

@end

#endif
