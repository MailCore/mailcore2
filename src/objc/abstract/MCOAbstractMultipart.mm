//
//  MCOAbstractMultipart.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOAbstractMultipart.h"

#include "MCAbstractMultipart.h"

#import "NSObject+MCO.h"

@implementation MCOAbstractMultipart

#define nativeType mailcore::AbstractMultipart

- (id) initWithCoder:(NSCoder *)decoder
{
	self = [super initWithCoder:decoder];
	
	self.parts = [[decoder decodeObjectForKey:@"parts"] retain];
	
	return self;
}

- (void) encodeWithCoder:(NSCoder *)encoder
{
	[encoder encodeObject:self.parts forKey:@"parts"];
}

MCO_OBJC_SYNTHESIZE_ARRAY(setParts, parts)

@end
