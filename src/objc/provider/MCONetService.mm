//
//  MCONetService.m
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCONetService.h"
#include "MCNetService.h"

#import "NSDictionary+MCO.h"
#import "NSString+MCO.h"

@implementation MCONetService {
	mailcore::NetService * _netService;
}

- (mailcore::Object *) mco_mcObject
{
    return _netService;
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::NetService * netService = (mailcore::NetService *) object;
    return [[self alloc] initWithNetService:netService];
}

+ (MCONetService *) netServiceWithInfo:(NSDictionary *)info
{
	return [[[MCONetService alloc] initWithInfo:info] autorelease];
}

- (id) initWithInfo:(NSDictionary *)info
{
	self = [super init];
	
	_netService = new mailcore::NetService(info.mco_mcHashMap);
	
	return self;
}

- (id) initWithNetService:(mailcore::NetService *)netService
{
	self = [super init];
	
	_netService = netService;
	
	return self;
}

- (void) setHostname:(NSString *)hostname {
	_netService->setHostname(hostname.mco_mcString);
}

- (NSString *) hostname {
	return [NSString mco_stringWithMCString:_netService->hostname()];
}

- (void) setPort:(unsigned int)port {
	_netService->setPort(port);
}

- (unsigned int) port {
	return _netService->port();
}

- (MCONetServiceConnectionType) connectionType {
	return (MCONetServiceConnectionType) _netService->connectionType();
}

- (NSDictionary *) info
{
	return [NSDictionary mco_dictionaryWithMCHashMap:_netService->info()];
}

- (NSString*)hostnameWithEmail:(NSString*)email
{
	NSString *result = nil;
	NSArray *components = [email componentsSeparatedByString:@"@"];
	if (components.count == 0) {
		return self.hostname;
	} else {
		result = [self.hostname stringByReplacingOccurrencesOfString:@"{domain}" withString:[components lastObject]];
	}
	return [NSString mco_stringWithMCString:_netService->normalizedHostnameWithEmail(email.mco_mcString)];
}

@end
