//
//  MCOIMAPFolderStatus.m
//  mailcore2
//
//  Created by Sebastian on 6/5/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPFolderStatus.h"


@implementation MCOIMAPFolderStatus {
    uint32_t total_unseen;
    uint32_t total_messages;
    uint32_t total_recent;
    uint32_t _uidNext;
    uint32_t _uidValidity;
}

@synthesize uidNext = _uidNext;
@synthesize uidValidity = _uidValidity;
@synthesize totalMessages = total_messages;
@synthesize totalRecent = total_recent;
@synthesize totalUnseen = total_unseen;



+ (MCOIMAPFolderStatus *) status
{
    return [[[MCOIMAPFolderStatus alloc] init] autorelease];
}



@end
