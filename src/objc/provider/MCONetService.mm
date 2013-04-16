//
//  MCONetService.m
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCONetService.h"

#include "MCOUtils.h"

#define MCOConnectionMask (mailcore::ConnectionTypeTLS | mailcore::ConnectionTypeStartTLS | mailcore::ConnectionTypeClear)

@implementation MCONetService 

- (id) init
{
    self = [super init];
    
    return self;
}

- (void) dealloc
{
	[_hostname release];
    [super dealloc];
}

- (id) copyWithZone:(NSZone *)zone
{
    return [[MCONetService netServiceWithInfo:[self info]] retain];
}

- (id) initWithInfo:(NSDictionary *)info
{
    BOOL ssl;
    BOOL starttls;
    
    self = [self init];
    
    [self setHostname:[info objectForKey:@"hostname"]];
    [self setPort:[[info objectForKey:@"port"] intValue]];
    ssl = [[info objectForKey:@"ssl"] boolValue];
    starttls = [[info objectForKey:@"starttls"] boolValue];
    if (ssl) {
        _authType = mailcore::ConnectionTypeTLS;
    }
    else if (starttls) {
        _authType = mailcore::ConnectionTypeStartTLS;
    }
    else {
        _authType = mailcore::ConnectionTypeClear;
    }
    
    return self;
}

- (NSDictionary *) info
{
    NSMutableDictionary * result;
    
    result = [NSMutableDictionary dictionary];
    if ([self hostname] != nil) {
        [result setObject:[self hostname] forKey:@"hostname"];
    }
    if ([self port] != 0) {
        [result setObject:[NSNumber numberWithInt:[self port]] forKey:@"port"];
    }
    switch (_authType & MCOConnectionMask) {
        case mailcore::ConnectionTypeTLS:
            [result setObject:[NSNumber numberWithBool:YES] forKey:@"ssl"];
            break;
        case mailcore::ConnectionTypeStartTLS:
            [result setObject:[NSNumber numberWithBool:YES] forKey:@"starttls"];
            break;
    }
    
    return result;
}

- (NSString*)hostnameWithEmail:(NSString*)email {
	NSString *result = nil;
	NSArray *components = [email componentsSeparatedByString:@"@"];
	if (components.count == 0) {
		return self.hostname;
	} else {
		result = [self.hostname stringByReplacingOccurrencesOfString:@"{domain}" withString:[components lastObject]];
	}
	return result;
}

+ (MCONetService *) netServiceWithInfo:(NSDictionary *)info
{
    return [[[MCONetService alloc] initWithInfo:info] autorelease];
}

@end
