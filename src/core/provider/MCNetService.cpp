//
//  MCONetService.cpp
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCNetService.h"

using namespace mailcore;

NetService::NetService() {
	init();
}

NetService::NetService(HashMap * info)
{
	init();
}

void NetService::init() {
	
}

NetService::~NetService()
{
	MC_SAFE_RELEASE(mHostname);
}

void NetService::setHostname(String * hostname)
{
	MC_SAFE_REPLACE_COPY(String, mHostname, hostname);
}

String * NetService::hostname()
{
	return mHostname;
}

void NetService::setPort(unsigned int port)
{
	mPort = port;
}

unsigned int NetService::port()
{
	return mPort;
}

void NetService::setAuthType(AuthType authType)
{
	mAuthType = authType;
}

AuthType NetService::authType()
{
	return mAuthType;
}

HashMap * NetService::info()
{
	HashMap * result;
    
    result = new HashMap();
    if (mHostname != NULL) {
        result->setObjectForKey(mHostname, MCSTR("hostname"));
    }
    if (mPort != 0) {
        result->setObjectForKey(Value::valueWithUnsignedIntValue(mPort), MCSTR("port"));
    }
    switch (mAuthType & (ConnectionTypeClear | ConnectionTypeStartTLS | ConnectionTypeTLS)) {
        case ConnectionTypeTLS:
			result->setObjectForKey(Value::valueWithBoolValue(true), MCSTR("ssl"));
            break;
        case ConnectionTypeStartTLS:
			result->setObjectForKey(Value::valueWithBoolValue(true), MCSTR("starttls"));
            break;
    }
    
    return result;
}
