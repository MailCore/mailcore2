//
//  MCPerfEvent.h
//  mailcore2
//
//  Created by Yuk Lai Suen on 5/30/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#ifndef __mailcore2__File__
#define __mailcore2__File__
#include <stdint.h>

#define TRACE_PERF MCPerfEvent event(__PRETTY_FUNCTION__)

namespace mailcore {
    class MCPerfEvent
    {
    public:
        MCPerfEvent(const char *functionName, bool start = true);
        ~MCPerfEvent();
        unsigned long long timeElapsedInMs;
    private:
        bool isStarted;
        const char *functionName;
        uint64_t startTime;
        uint64_t endTime;
    };
}
#endif /* defined(__mailcore2__File__) */
