//
//  MCConnectionLogger.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 6/24/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_CONNECTION_LOGGER_H_
#define __MAILCORE_CONNECTION_LOGGER_H_

#include <stdlib.h>

#ifdef __cplusplus

namespace mailcore {
    
    class Data;
    
    enum ConnectionLogType {
        // Received data
        ConnectionLogTypeReceived,
        // Sent data
        ConnectionLogTypeSent,
        // Sent private data
        ConnectionLogTypeSentPrivate,
        // Parse error
        ConnectionLogTypeErrorParse,
        // Error while receiving data - log() is called with a NULL buffer.
        ConnectionLogTypeErrorReceived,
        // Error while sending data - log() is called with a NULL buffer.
        ConnectionLogTypeErrorSent,
    };
    
    class ConnectionLogger {
    public:
        virtual void log(void * sender, ConnectionLogType logType, Data * buffer) {}
    };
    
}

#endif

#endif
