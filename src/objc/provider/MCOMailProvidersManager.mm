//
//  MCOMailProvidersManager.m
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMailProvidersManager.h"
#include "MCOMailProvider.h"
#import "MCOUtils.h"
#import "MCOObjectWrapper.h"

@interface MCOMailProvidersManager ()

@property (nonatomic, strong) NSMutableDictionary *providers;

@end

@implementation MCOMailProvidersManager 

+ (instancetype) sharedManager
{
	static MCOMailProvidersManager *sharedInstance = nil;
	static dispatch_once_t onceToken;
	dispatch_once(&onceToken, ^{
		sharedInstance = [[MCOMailProvidersManager alloc] init];
	});
	return sharedInstance;
}

- (id) init
{
    NSString * filename;
    
    self = [super init];
    
    _providers = [[NSMutableDictionary alloc] init];
    
    filename =  [[NSBundle bundleForClass:[self class]] pathForResource:@"providers" ofType:@"plist"];
    [self registerProvidersFilename:filename];
    
    return self;
}

- (void) dealloc
{
    [_providers release];
    [super dealloc];
}

- (void) registerProviders:(NSDictionary *)providers
{
    for(NSString * identifier in providers) {
        MCOMailProvider * provider;
        
        provider = [[MCOMailProvider alloc] initWithInfo:[providers objectForKey:identifier]];
        [provider setIdentifier:identifier];
        [_providers setObject:provider forKey:identifier];
        [provider release];
    }
}

- (void) registerProvidersFilename:(NSString *)filename
{
    NSDictionary * providersInfos;
    
    providersInfos = [[NSDictionary alloc] initWithContentsOfFile:filename];
    [self registerProviders:providersInfos];
    [providersInfos release];
}

- (MCOMailProvider *) providerForEmail:(NSString *)email
{
    for(NSString * identifier in _providers) {
        MCOMailProvider * provider;
        
        provider = [_providers objectForKey:identifier];
        if ([provider matchEmail:email])
            return provider;
    }
    
    return nil;
}

- (MCOMailProvider *) providerForMX:(NSString *)hostname
{
    for(NSString * identifier in _providers) {
        MCOMailProvider * provider;
        
        provider = [_providers objectForKey:identifier];
        if ([provider matchMX:[hostname lowercaseString]])
            return provider;
    }
    
    return nil;
}

- (MCOMailProvider *) providerForIdentifier:(NSString *)identifier
{
    return [_providers objectForKey:identifier];
}

@end