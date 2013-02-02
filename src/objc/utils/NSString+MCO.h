//
//  NSString+MCO.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/21/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef __cplusplus
namespace mailcore {
    class String;
    class Object;
}
#endif

@interface NSString (MCO)

#ifdef __cplusplus
+ (NSString *) mco_stringWithMCString:(mailcore::String *)cppString;
+ (NSString *) mco_stringWithMCObject:(mailcore::Object *)object;

- (mailcore::String *) mco_mcString;
#endif

@end
