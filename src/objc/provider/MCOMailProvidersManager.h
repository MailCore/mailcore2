//
//  MCOMailProvidersManager.h
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MCOMailProvider;

// This class manages the plist for providers.

@interface MCOMailProvidersManager : NSObject

// Returns the shared providers manager instance.
+ (instancetype) sharedManager;

// Returns the provider for the given email address, or nil if no match is found.
- (MCOMailProvider *) providerForEmail:(NSString *)email;

// Returns the provider for the given MX, or nil if no match is found.
- (MCOMailProvider *) providerForMX:(NSString *)hostname;

// Returns the provider for the given identifier, or nil if no match is found.
- (MCOMailProvider *) providerForIdentifier:(NSString *)identifier;

// Adds the given providers to the internal dictionary of providers.
// See providers.plist for the class' serialized format.
- (void) registerProviders:(NSDictionary *)providers;

// A convenience for registering a given plist to the internal dictionary of providers.
// See providers.plist for the class' serialized format.
- (void) registerProvidersFilename:(NSString *)filename;

@end
