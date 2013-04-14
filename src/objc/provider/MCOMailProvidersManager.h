//
//  MCOMailProvidersManager.h
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MCOMailProvider;

@interface MCOMailProvidersManager : NSObject {

}

+ (instancetype) sharedManager;

- (MCOMailProvider *) providerForEmail:(NSString *)email;
- (MCOMailProvider *) providerForMX:(NSString *)hostname;
- (MCOMailProvider *) providerForIdentifier:(NSString *)identifier;
- (void) registerProviders:(NSDictionary *)providers;
- (void) registerProvidersFilename:(NSString *)filename;

@end
