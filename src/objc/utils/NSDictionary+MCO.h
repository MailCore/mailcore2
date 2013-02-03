//
//  NSDictionary+MCO.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/29/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef __cplusplus
namespace mailcore {
    class HashMap;
}
#endif

@interface NSDictionary (MCO)

#ifdef __cplusplus
- (mailcore::HashMap *) mco_mcHashMap;
#endif

@end
