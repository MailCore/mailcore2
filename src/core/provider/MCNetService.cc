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
	
	bool ssl = false;
    bool starttls = false;
	
	this->setHostname((String *) info->objectForKey(MCSTR("hostname")));
    if (info->objectForKey(MCSTR("port")) != NULL) {
        this->setPort(((Value *) info->objectForKey(MCSTR("port")))->intValue());
    }
    if (info->objectForKey(MCSTR("ssl")) != NULL) {
        ssl = ((Value *) info->objectForKey(MCSTR("ssl")))->boolValue();
    }
    if (info->objectForKey(MCSTR("starttls")) != NULL) {
        starttls = ((Value *) info->objectForKey(MCSTR("starttls")))->boolValue();
    }
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
		result->setObjectForKey(MCSTR("hostname"), mHostname);
    }
    if (mPort != 0) {
		result->setObjectForKey(MCSTR("port"), Value::valueWithIntValue(mPort));
    }
    switch (mConnectionType) {
        case ConnectionTypeTLS:
			result->setObjectForKey(MCSTR("ssl"), Value::valueWithBoolValue(true));
            break;
        case ConnectionTypeStartTLS:
			result->setObjectForKey(MCSTR("starttls"), Value::valueWithBoolValue(true));
            break;
        default:
            break;
    }
    
    return result;
}
