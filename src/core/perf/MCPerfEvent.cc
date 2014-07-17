//
//  MCPerfEvent.cpp
//  mailcore2
//
//  Created by Yuk Lai Suen on 5/30/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#include <stdint.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include "MCLog.h"
#include "MCPerfEvent.h"

static struct mach_timebase_info sTimebaseInfo;

using namespace mailcore;
MCPerfEvent::MCPerfEvent(const char *_functionName, bool start)
: isStarted(false)
, functionName(_functionName)
{
    if (start) {
        if (sTimebaseInfo.denom == 0) {
            mach_timebase_info(&sTimebaseInfo);
        }
        startTime = mach_absolute_time();
        isStarted = true;
    }
}

MCPerfEvent::~MCPerfEvent()
{
    if (isStarted) {
        endTime = mach_absolute_time();
        isStarted = false;
    }
    
    uint64_t timeElpasedInMs = ((endTime - startTime) / 1000000ull * sTimebaseInfo.numer / sTimebaseInfo.denom);
    MCLog("================= %s: %llu (ms)", functionName, timeElpasedInMs);
}
