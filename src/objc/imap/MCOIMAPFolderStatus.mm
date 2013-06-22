//
//  MCOIMAPFolderStatus.m
//  mailcore2
//
//  Created by Sebastian on 6/5/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPFolderStatus.h"
#import "NSObject+MCO.h"

#include "MCIMAPFolderStatus.h"

#define nativeType mailcore::IMAPFolderStatus

@implementation MCOIMAPFolderStatus {
	mailcore::IMAPFolderStatus *_folderStatus;
}

+ (MCOIMAPFolderStatus *) status
{
	mailcore::IMAPFolderStatus folderStatus = new mailcore::IMAPFolderStatus();
    return [[[MCOIMAPFolderStatus alloc] init] autorelease];
}

- (id) initWithMCFolderStatus:(mailcore::IMAPFolderStatus *)status
{
    self = [super init];
    
    status->retain();
    _folderStatus = status;
    
    return self;
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::IMAPFolderStatus * status = (mailcore::IMAPFolderStatus *) object;
    return [[[self alloc] initWithMCFolderStatus:status] autorelease];
}

- (id) copyWithZone:(NSZone *)zone
{
    nativeType * nativeObject = (nativeType *) [self mco_mcObject]->copy();
    id result = [[self class] mco_objectWithMCObject:nativeObject];
    MC_SAFE_RELEASE(nativeObject);
    return [result retain];
}

MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setUnseenCount, unseenCount)
MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setMessageCount, messageCount)
MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setRecentCount, recentCount)
MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setUidNext, uidNext)
MCO_OBJC_SYNTHESIZE_SCALAR(uint32_t, uint32_t, setUidValidity, uidValidity)

- (void) dealloc
{
	_folderStatus->release();
    [super dealloc];
}

@end
