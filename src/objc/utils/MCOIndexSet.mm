//
//  MCOIndexSet.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/23/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIndexSet.h"

#include <mailcore/MCBaseTypes.h>

#import "NSObject+MCO.h"

@implementation MCOIndexSet {
    mailcore::IndexSet * _indexSet;
}

#define nativeType mailcore::IndexSet

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

+ (id) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::IndexSet * part = (mailcore::IndexSet *) object;
    return [[[self alloc] initWithMCIndexSet:part] autorelease];
}

- (id) init
{
    mailcore::IndexSet * indexSet = new mailcore::IndexSet();
    self = [self initWithMCIndexSet:indexSet];
    indexSet->release();
    return self;
}

- (id) initWithMCIndexSet:(mailcore::IndexSet *)indexSet
{
    self = [super init];
    _indexSet = indexSet;
    _indexSet->retain();
    return self;
}

- (void) dealloc
{
    MC_SAFE_RELEASE(_indexSet);
    [super dealloc];
}

+ (MCOIndexSet *) indexSet
{
    return [[[MCOIndexSet alloc] init] autorelease];
}

+ (MCOIndexSet *) indexSetWithRange:(MCORange)range
{
    MCOIndexSet * indexSet;
    indexSet = [[[MCOIndexSet alloc] init] autorelease];
    [indexSet addRange:range];
    return indexSet;
}

+ (MCOIndexSet *) indexSetWithIndex:(uint64_t)idx
{
    MCOIndexSet * indexSet;
    indexSet = [[[MCOIndexSet alloc] init] autorelease];
    [indexSet addIndex:idx];
    return indexSet;
}

- (NSString *) description
{
    return MCO_OBJC_BRIDGE_GET(description);
}

- (unsigned int) count
{
    return _indexSet->count();
}

- (void) addIndex:(uint64_t)idx
{
    _indexSet->addIndex(idx);
}

- (void) removeIndex:(uint64_t)idx
{
    _indexSet->removeIndex(idx);
}

- (BOOL) containsIndex:(uint64_t)idx
{
    return _indexSet->containsIndex(idx);
}

- (void) addRange:(MCORange)range
{
    _indexSet->addRange(MCORangeToMCRange(range));
}

- (void) removeRange:(MCORange)range
{
    _indexSet->removeRange(MCORangeToMCRange(range));
}

- (void) intersectsRange:(MCORange)range
{
    _indexSet->intersectsRange(MCORangeToMCRange(range));
}

- (MCORange *) allRanges
{
#if 0
    NSMutableData * result = [[[NSMutableData alloc] init] autorelease];
    unsigned int count = _indexSet->rangesCount();
    mailcore::Range * mcRanges = _indexSet->allRanges();
    [result setLength:count * sizeof(mailcore::Range)];
    MCORange * ranges = (MCORange *) [result mutableBytes];
    for(unsigned int i = 0 ; i < count ; i ++) {
        ranges[i] = MCORangeWithMCRange(mcRanges[i]);
    }
    return ranges;
#else
    return (MCORange *) _indexSet->allRanges();
#endif
}

- (unsigned int) rangesCount
{
    return _indexSet->rangesCount();
}

@end
