//
//  NSObject+MCO.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef __cplusplus
namespace mailcore {
    class Object;
}
#endif

@interface NSObject (MCO)

#ifdef __cplusplus
- (mailcore::Object *) mco_mcObject;
#endif

@end
