//
//  MCMailProvidersManager.cpp
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCMailProvidersManager.h"
#include "MCMailProvider.h"
#include "MCJSON.h"

using namespace mailcore;

void MailProvidersManager::init()
{
	mProviders = new HashMap();
}

MailProvider * MailProvidersManager::providerForEmail(String * email)
{
	mc_foreachdictionaryValue(MailProvider, provider, mProviders) {
        if (provider->matchEmail(email))
            return provider;
	}
	
	return NULL;
}

MailProvider * MailProvidersManager::providerForMX(String * hostname)
{
	mc_foreachdictionaryValue(MailProvider, provider, mProviders) {
        if (provider->matchMX(hostname))
            return provider;
	}
	
	return NULL;
}

MailProvider * MailProvidersManager::providerForIdentifier(String * identifier)
{
	return (MailProvider *) mProviders->objectForKey(identifier);
}

void MailProvidersManager::registerProviders(HashMap * providers)
{
	mc_foreachdictionaryKeyAndValue(String, identifier, HashMap, providerInfo, providers) {
        MailProvider * provider = new MailProvider(providerInfo);
        provider->setIdentifier(identifier);
        MCLog("register %s", MCUTF8DESC(identifier));
        mProviders->setObjectForKey(identifier, provider);
        provider->release();
	}
}

void MailProvidersManager::registerProvidersFilename(String * filename)
{
	HashMap * providersInfos;
    
    providersInfos = (HashMap *) JSON::objectFromJSONData(Data::dataWithContentsOfFile(filename));
    registerProviders(providersInfos);
    providersInfos->release();
}
