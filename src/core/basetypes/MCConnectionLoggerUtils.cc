//
//  MCConnectionLoggerUtils.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 6/24/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCConnectionLoggerUtils.h"

#include <libetpan/libetpan.h>

#include "MCConnectionLogger.h"

mailcore::ConnectionLogType mailcore::getConnectionType(int log_type)
{
    ConnectionLogType type = (ConnectionLogType) -1;
    bool isBuffer = false;
    
    switch (log_type) {
        case MAILSTREAM_LOG_TYPE_ERROR_PARSE:
            type = ConnectionLogTypeErrorParse;
            isBuffer = true;
            break;
        case MAILSTREAM_LOG_TYPE_ERROR_RECEIVED:
            type = ConnectionLogTypeErrorReceived;
            isBuffer = true;
            break;
        case MAILSTREAM_LOG_TYPE_ERROR_SENT:
            type = ConnectionLogTypeErrorSent;
            isBuffer = true;
            break;
        case MAILSTREAM_LOG_TYPE_DATA_RECEIVED:
            type = ConnectionLogTypeReceived;
            isBuffer = true;
            break;
        case MAILSTREAM_LOG_TYPE_DATA_SENT:
            type = ConnectionLogTypeSent;
            isBuffer = true;
            break;
        case MAILSTREAM_LOG_TYPE_DATA_SENT_PRIVATE:
            type = ConnectionLogTypeSentPrivate;
            isBuffer = true;
            break;
    }
    return type;
}

bool mailcore::isBufferFromLogType(int log_type)
{
    bool isBuffer = false;
    
    switch (log_type) {
        case MAILSTREAM_LOG_TYPE_ERROR_PARSE:
        case MAILSTREAM_LOG_TYPE_DATA_RECEIVED:
        case MAILSTREAM_LOG_TYPE_DATA_SENT:
        case MAILSTREAM_LOG_TYPE_DATA_SENT_PRIVATE:
            isBuffer = true;
            break;
    }
    return isBuffer;
}
