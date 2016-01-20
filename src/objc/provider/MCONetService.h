//
//  MCONetService.h
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MailCore/MCOConstants.h>

/**
   This class provides a specific way to access a given service
*/

@interface MCONetService : NSObject <NSCopying>

/**
   Creates and initializes a `MCONetService` object with the info dictionary.
*/
+ (MCONetService *) serviceWithInfo:(NSDictionary *)info;

/**
   Initializes a `MCONetService` object with the given info dictionary.
*/
- (id) initWithInfo:(NSDictionary *)info NS_DESIGNATED_INITIALIZER;

/**
   The hostname of the server. [MCONetService hostnameWithEmail:] is recommended
   instead as it can handle services with custom domains 
*/
@property (nonatomic, copy) NSString * hostname;

/** The port number of the service */
@property (nonatomic, assign) unsigned int port;

/** What kind of connection type is supported, like SSL, Start TLS, Plain etc. */
@property (nonatomic, assign) MCOConnectionType connectionType;

- (NSDictionary *) info;

/** 
    If the service uses a custom domain this will return the proper hostname based
    off the email address
*/
- (NSString *) hostnameWithEmail:(NSString *)email;

@end

@interface MCONetService (MCOUnavailable)

- (id) init NS_UNAVAILABLE;

@end
