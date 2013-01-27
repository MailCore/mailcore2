//
//  MCOObjectWrapper.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

#include <mailcore/MCObject.h>

@interface MCOObjectWrapper : NSObject

#ifdef __cplusplus
@property (nonatomic, assign) mailcore::Object * object;

+ (MCOObjectWrapper *) objectWrapperWithObject:(mailcore::Object *)object;
#endif

@end
