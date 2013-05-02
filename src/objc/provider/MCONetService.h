//
//  MCONetService.h
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum {
	MCONetServiceConnectionTypeClear             = 1 << 0,
	MCONetServiceConnectionTypeStartTLS          = 1 << 1,
	MCONetServiceConnectionTypeTLS               = 1 << 2,
	MCONetServiceConnectionTypeMask				 = (MCONetServiceConnectionTypeClear | MCONetServiceConnectionTypeStartTLS | MCONetServiceConnectionTypeTLS)
} MCONetServiceConnectionType;

@interface MCONetService : NSObject

@property (nonatomic, copy) NSString * hostname;
@property (nonatomic, assign) unsigned int port;
@property (nonatomic, assign) MCONetServiceConnectionType connectionType;

+ (MCONetService *) netServiceWithInfo:(NSDictionary *)info;

- (id) initWithInfo:(NSDictionary *)info;
- (NSDictionary *) info;

@end
