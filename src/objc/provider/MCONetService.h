//
//  MCONetService.h
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MailCore/MCOConstants.h>

@interface MCONetService : NSObject <NSCopying>

@property (nonatomic, copy) NSString * hostname;
@property (nonatomic, assign) unsigned int port;
@property (nonatomic, assign) MCOConnectionType connectionType;

+ (MCONetService *) serviceWithInfo:(NSDictionary *)info;

- (id) initWithInfo:(NSDictionary *)info;
- (NSDictionary *) info;

- (NSString *) hostnameWithEmail:(NSString *)email;

@end
