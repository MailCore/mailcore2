//
//  MCOIMAPSearchExpression.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPSearchExpression.h"

#include "MCIMAP.h"

#import "MCOUtils.h"

@implementation MCOIMAPSearchExpression {
    mailcore::IMAPSearchExpression * _nativeExpr;
}

#define nativeType mailcore::IMAPSearchExpression

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

- (id) copyWithZone:(NSZone *)zone
{
    nativeType * nativeObject = (nativeType *) [self mco_mcObject]->copy();
    id result = [[self class] mco_objectWithMCObject:nativeObject];
    MC_SAFE_RELEASE(nativeObject);
    return [result retain];
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::IMAPSearchExpression * expr = (mailcore::IMAPSearchExpression *) object;
    return [[[self alloc] initWithMCExpression:expr] autorelease];
}

- (mailcore::Object *) mco_mcObject
{
    return _nativeExpr;
}

- (id) initWithMCExpression:(mailcore::IMAPSearchExpression *)expr
{
    self = [super init];
    
    _nativeExpr = (mailcore::IMAPSearchExpression *) expr->copy();
    
    return self;
}

- (void) dealloc
{
    MC_SAFE_RELEASE(_nativeExpr);
    [super dealloc];
}

+ (MCOIMAPSearchExpression *) searchAll
{
    return MCO_TO_OBJC(mailcore::IMAPSearchExpression::searchAll());
}

+ (MCOIMAPSearchExpression *) searchFrom:(NSString *)value
{
    return MCO_TO_OBJC(mailcore::IMAPSearchExpression::searchFrom([value mco_mcString]));
}

+ (MCOIMAPSearchExpression *) searchRecipient:(NSString *)value
{
    return MCO_TO_OBJC(mailcore::IMAPSearchExpression::searchRecipient([value mco_mcString]));
}

+ (MCOIMAPSearchExpression *) searchSubject:(NSString *)value
{
    return MCO_TO_OBJC(mailcore::IMAPSearchExpression::searchSubject([value mco_mcString]));
}

+ (MCOIMAPSearchExpression *) searchContent:(NSString *)value
{
    return MCO_TO_OBJC(mailcore::IMAPSearchExpression::searchContent([value mco_mcString]));
}

+ (MCOIMAPSearchExpression *) searchHeader:(NSString *)header value:(NSString *)value
{
    return MCO_TO_OBJC(mailcore::IMAPSearchExpression::searchHeader([header mco_mcString], [value mco_mcString]));
}

+ (MCOIMAPSearchExpression *) searchGmailThreadId:(uint64_t)number
{
    return MCO_TO_OBJC(mailcore::IMAPSearchExpression::searchGmailThreadId(number));
}

+ (MCOIMAPSearchExpression *) searchAnd:(MCOIMAPSearchExpression *)expression other:(MCOIMAPSearchExpression *)other
{
    mailcore::IMAPSearchExpression * result = mailcore::IMAPSearchExpression::searchAnd(expression->_nativeExpr, other->_nativeExpr);
    return MCO_TO_OBJC(result);
}

+ (MCOIMAPSearchExpression *) searchOr:(MCOIMAPSearchExpression *)expression other:(MCOIMAPSearchExpression *)other
{
    mailcore::IMAPSearchExpression * result = mailcore::IMAPSearchExpression::searchOr(expression->_nativeExpr, other->_nativeExpr);
    return MCO_TO_OBJC(result);
}

@end
