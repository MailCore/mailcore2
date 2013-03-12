//
//  MCOAddress+Private.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/11/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifdef __cplusplus
namespace mailcore {
    class Address;
}

@interface MCOAddress (Private)

- (id) initWithMCAddress:(mailcore::Address *)address;
+ (MCOAddress *) addressWithMCAddress:(mailcore::Address *)address;

@end
#endif
