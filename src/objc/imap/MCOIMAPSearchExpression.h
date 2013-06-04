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
