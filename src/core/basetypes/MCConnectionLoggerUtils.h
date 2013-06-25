//
//  MCConnectionLoggerUtils.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 6/24/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCCONNECTIONLOGGERUTILS_H_
#define __MAILCORE_MCCONNECTIONLOGGERUTILS_H_

#include <MailCore/MCConnectionLogger.h>

namespace mailcore {

    ConnectionLogType getConnectionType(int log_type);
    bool isBufferFromLogType(int log_type);
    
}

#endif
