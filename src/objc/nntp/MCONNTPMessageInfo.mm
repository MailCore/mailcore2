//
//  MCONNTPMessageInfo.m
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#import "MCONNTPMessageInfo.h"

#include "MCAsyncNNTP.h"
#include "MCNNTP.h"

#import "MCOUtils.h"

@implementation MCONNTPMessageInfo {
    mailcore::NNTPMessageInfo * _nativeInfo;
}

#define nativeType mailcore::NNTPMessageInfo

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

- (id) copyWithZone:(NSZone *)zone
{
    nativeType * nativeObject = (nativeType *) [self mco_mcObject]->copy();
    id result = [[self class] mco_objectWithMCObject:nativeObject];
    MC_SAFE_RELEASE(nativeObject);
    return [result retain];
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::NNTPMessageInfo * folder = (mailcore::NNTPMessageInfo *) object;
    return [[[self alloc] initWithMCNNTPMessageInfo:folder] autorelease];
}

- (mailcore::Object *) mco_mcObject
{
    return _nativeInfo;
}

- (NSString *) description
{
    return MCO_OBJC_BRIDGE_GET(description);
}

- (id) initWithMCNNTPMessageInfo:(mailcore::NNTPMessageInfo *)info
{
    self = [super init];
    
    _nativeInfo = info;
    _nativeInfo->retain();
    
    return self;
}

- (void) dealloc
{
    MC_SAFE_RELEASE(_nativeInfo);
    [super dealloc];
}

MCO_OBJC_SYNTHESIZE_SCALAR(unsigned int, unsigned int, setIndex, index)
MCO_OBJC_SYNTHESIZE_SCALAR(unsigned int, unsigned int, setSize, size)
MCO_OBJC_SYNTHESIZE_STRING(setUid, uid)

@end
