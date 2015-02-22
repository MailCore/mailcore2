//
//  MCDataMac.m
//  mailcore2
//
//  Created by Hoa V. DINH on 10/24/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#include "MCData.h"

#import <Foundation/Foundation.h>

using namespace mailcore;

CFDataRef Data::destructiveNSData()
{
    NSData * result = [NSData dataWithBytesNoCopy:(void *) mBytes length:mLength];
    mBytes = NULL;
    mAllocated = 0;
    mLength = 0;
    return (CFDataRef) result;
}
