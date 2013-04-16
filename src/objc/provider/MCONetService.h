//
//  MCONetService.h
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "MCMessageConstants.h"

@interface MCONetService : NSObject <NSCopying> 

@property (nonatomic, copy) NSString * hostname;
@property (nonatomic, assign) unsigned int port;
@property (nonatomic, assign) mailcore::ConnectionType authType;

+ (MCONetService *) netServiceWithInfo:(NSDictionary *)info;

- (id) initWithInfo:(NSDictionary *)info;
- (NSDictionary *) info;

@end
