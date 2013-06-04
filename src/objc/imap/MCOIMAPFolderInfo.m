//
//  MCOIMAPFolderInfo.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/25/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPFolderInfo.h"

@implementation MCOIMAPFolderInfo {
    uint32_t _uidNext;
    uint32_t _uidValidity;
    uint64_t _modSequenceValue;
    int _messageCount;
    int _unseenCount;
}

@synthesize uidNext = _uidNext;
@synthesize uidValidity = _uidValidity;
@synthesize modSequenceValue = _modSequenceValue;
@synthesize messageCount = _messageCount;
@synthesize unseenCount = _unseenCount;

+ (MCOIMAPFolderInfo *) info
{
    return [[[MCOIMAPFolderInfo alloc] init] autorelease];
}

@end
