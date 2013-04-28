//
//  MCNetService.cpp
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCNetService.h"

using namespace mailcore;

void NetService::init() {
		
}

NetService::NetService(HashMap * info) {
	init();
	
	bool ssl;
    bool starttls;
	
	this->setHostname((String *) info->objectForKey(MCSTR("hostname")));
    this->setPort(((Value *) info->objectForKey(MCSTR("port")))->intValue());
	ssl = ((Value *) info->objectForKey(MCSTR("ssl")))->boolValue();
	starttls = ((Value *) info->objectForKey(MCSTR("starttls")))->boolValue();
    if (ssl) {
        mConnectionType = ConnectionTypeTLS;
    }
    else if (starttls) {
        mConnectionType = ConnectionTypeStartTLS;
    }
    else {
        mConnectionType = ConnectionTypeClear;
    }
}

NetService::~NetService() {
	delete mHostname;
}

void NetService::setHostname(String *hostname)
{
	mHostname = hostname;
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

void NetService::setConnectionType(ConnectionType connectionType)
{
	mConnectionType = connectionType;
}

ConnectionType NetService::connectionType()
{
	return mConnectionType;
}

String * NetService::normalizedHostnameWithEmail(String * email) {
	Array *components = email->componentsSeparatedByString(MCSTR("@"));
	String *hostname = (String *) mHostname->copy();
	if (components->count() != 0) {
		hostname->replaceOccurrencesOfString(MCSTR("{domain}"), (String *) components->lastObject());
		return hostname;
	}
	return mHostname;
}

HashMap * NetService::info() {
	HashMap * result;
    
    result = new HashMap();
    if (mHostname != NULL) {
		result->setObjectForKey(mHostname, MCSTR("hostname"));
    }
    if (mPort != 0) {
		result->setObjectForKey(Value::valueWithIntValue(mPort), MCSTR("port"));
    }
    switch (mConnectionType & ConnectionTypeMask) {
        case ConnectionTypeTLS:
			result->setObjectForKey(Value::valueWithBoolValue(true), MCSTR("ssl"));
            break;
        case ConnectionTypeStartTLS:
			result->setObjectForKey(Value::valueWithBoolValue(true), MCSTR("starttls"));
            break;
    }
    
    return result;
}
