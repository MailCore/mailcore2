//
//  MCOIMAPSearchExpression.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <mailcore/MCOConstants.h>

#ifndef __MAILCORE_MCOIMAPSEARCHEXPRESSION_H_

#define __MAILCORE_MCOIMAPSEARCHEXPRESSION_H_

@interface MCOIMAPSearchExpression : NSObject

+ (MCOIMAPSearchExpression *) searchFrom:(NSString *)value;
+ (MCOIMAPSearchExpression *) searchRecipient:(NSString *)value;
+ (MCOIMAPSearchExpression *) searchSubject:(NSString *)value;
+ (MCOIMAPSearchExpression *) searchContent:(NSString *)value;
+ (MCOIMAPSearchExpression *) searchHeader:(NSString *)header value:(NSString *)value;
+ (MCOIMAPSearchExpression *) searchAnd:(MCOIMAPSearchExpression *)expression other:(MCOIMAPSearchExpression *)other;
+ (MCOIMAPSearchExpression *) searchOr:(MCOIMAPSearchExpression *)expression other:(MCOIMAPSearchExpression *)other;

@end

#endif
