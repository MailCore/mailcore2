//
//  NSData+MCO.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/21/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

#include <mailcore/mailcore.h>

@interface NSData (MCO)

#ifdef __cplusplus
+ (NSData *) mco_dataWithMCData:(mailcore::Data *)cppData;
#endif

@end
