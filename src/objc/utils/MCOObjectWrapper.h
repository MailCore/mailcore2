//
//  MCOObjectWrapper.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCOOBJECTWRAPPER_H_

#define __MAILCORE_MCOOBJECTWRAPPER_H_

#import <Foundation/Foundation.h>

#ifdef __cplusplus
namespace mailcore {
    class Object;
}
#endif

@interface MCOObjectWrapper : NSObject

#ifdef __cplusplus
@property (nonatomic, assign) mailcore::Object * object;

+ (MCOObjectWrapper *) objectWrapperWithObject:(mailcore::Object *)object;
#endif

@end

#endif
