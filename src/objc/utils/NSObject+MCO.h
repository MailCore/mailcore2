//
//  NSObject+MCO.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef __cplusplus
#include <typeinfo>
#endif

#ifdef __cplusplus
namespace mailcore {
    class Object;
}
#endif

@interface NSObject (MCO)

#ifdef __cplusplus
+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object;

- (mailcore::Object *) mco_mcObject;
#endif

@end

#ifdef __cplusplus
extern void MCORegisterClass(Class aClass, const std::type_info * info);
#endif

