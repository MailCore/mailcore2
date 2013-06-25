//
//  MCConnectionLogger.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 6/24/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_CONNECTION_LOGGER_H_
#define __MAILCORE_CONNECTION_LOGGER_H_

#ifdef __cplusplus

namespace mailcore {
    
    class Data;
    class String;
    
    enum ConnectionLogType {
        ConnectionLogTypeGeneric,
        ConnectionLogTypeReceived,
        ConnectionLogTypeSent,
        ConnectionLogTypeSentPrivate,
        ConnectionLogTypeErrorGeneric,
        ConnectionLogTypeErrorReceived,
        ConnectionLogTypeErrorSent,
        ConnectionLogTypeErrorSentPrivate,
    };
    
    class ConnectionLogger {
    public:
        virtual void logBuffer(ConnectionLogType logType, Data * buffer) {}
        virtual void logString(ConnectionLogType logType, String * log) {}
    };
    
}

#endif

#endif
