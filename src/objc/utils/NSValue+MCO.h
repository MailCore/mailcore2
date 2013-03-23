//
//  NSNumber+MCO.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/21/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef __cplusplus
namespace mailcore {
    class Value;
}
#endif

@interface NSValue (MCO)

#ifdef __cplusplus
+ (NSValue *) mco_valueWithMCValue:(mailcore::Value *)value;

- (mailcore::Value *) mco_mcValue;
#endif

@end
