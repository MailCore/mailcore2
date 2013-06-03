//
//  MCMailProvider.cpp
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCMailProvider.h"
#include "MCNetService.h"
#include "MCIterator.h"
#include "MCJSON.h"

#include <regex.h>

using namespace mailcore;

void MailProvider::init()
{
    mIdentifier = NULL;
	mImapServices = new Array();
    mSmtpServices = new Array();
    mPopServices = new Array();
    mDomainMatch = new Array();
    mMxSet = new Set();
    mMailboxPaths = NULL;
}

MailProvider::MailProvider()
{
    init();
}

MailProvider::~MailProvider()
{
	MC_SAFE_RELEASE(mImapServices);
	MC_SAFE_RELEASE(mSmtpServices);
	MC_SAFE_RELEASE(mPopServices);
	MC_SAFE_RELEASE(mMxSet);
	MC_SAFE_RELEASE(mDomainMatch);
	MC_SAFE_RELEASE(mMailboxPaths);
	MC_SAFE_RELEASE(mIdentifier);
}

MailProvider * MailProvider::providerWithInfo(HashMap * info)
{
    MailProvider * provider = new MailProvider();
    provider->fillWithInfo(info);
    provider->autorelease();
    return provider;
}

void MailProvider::fillWithInfo(HashMap * info)
{
	Array * imapInfos;
    Array * smtpInfos;
    Array * popInfos;
    HashMap * serverInfo;
    Array * mxs;
    
    MC_SAFE_RELEASE(mDomainMatch);
    if (info->objectForKey(MCSTR("domain-match")) != NULL) {
        mDomainMatch = (Array *) info->objectForKey(MCSTR("domain-match"))->retain();
    }
    MC_SAFE_RELEASE(mMailboxPaths);
    if (info->objectForKey(MCSTR("mailboxes")) != NULL) {
        mMailboxPaths = (HashMap *) info->objectForKey(MCSTR("mailboxes"))->retain();
    }
    mxs = (Array *) info->objectForKey(MCSTR("mx"));
    mMxSet->removeAllObjects();
	mc_foreacharray(String, mx, mxs) {
		mMxSet->addObject(mx->lowercaseString());
	}
    
    serverInfo = (HashMap *) info->objectForKey(MCSTR("servers"));
    imapInfos = (Array *) serverInfo->objectForKey(MCSTR("imap"));
    smtpInfos = (Array *) serverInfo->objectForKey(MCSTR("smtp"));
    popInfos = (Array *) serverInfo->objectForKey(MCSTR("pop"));
    
    mImapServices->removeAllObjects();
	mc_foreacharray(HashMap, imapInfo, imapInfos) {
		NetService * service = NetService::serviceWithInfo(imapInfo);
        mImapServices->addObject(service);
	}
	
    mSmtpServices->removeAllObjects();
	mc_foreacharray(HashMap, smtpInfo, smtpInfos) {
		NetService * service = NetService::serviceWithInfo(smtpInfo);
		mSmtpServices->addObject(service);
	}
	
    mPopServices->removeAllObjects();
	mc_foreacharray(HashMap, popInfo, popInfos) {
		NetService * service = NetService::serviceWithInfo(popInfo);
		mPopServices->addObject(service);
	}
}

void MailProvider::setIdentifier(String * identifier)
{
	MC_SAFE_REPLACE_COPY(String, mIdentifier, identifier);
}

String * MailProvider::identifier()
{
	return mIdentifier;
}

Array * MailProvider::imapServices()
{
	return mImapServices;
}

Array * MailProvider::smtpServices()
{
	return mSmtpServices;
}

Array * MailProvider::popServices()
{
	return mPopServices;
}

bool MailProvider::matchEmail(String * email)
{
	Array * components;
    String * domain;
    const char * cDomain;
    
    components = email->componentsSeparatedByString(MCSTR("@"));
    if (components->count() < 2)
        return false;
    
    domain = (String *) components->lastObject();
    cDomain = domain->UTF8Characters();
	
	mc_foreacharray(String, match, mDomainMatch) {
		regex_t r;
        bool matched;
        
        match = String::stringWithUTF8Format("^%s$", match->UTF8Characters());
        if (regcomp(&r, match->UTF8Characters(), REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0)
            continue;
        
        matched = false;
        if (regexec(&r, cDomain, 0, NULL, 0) == 0) {
            matched = true;
        }
        
        regfree(&r);
        
        if (matched)
            return true;
	}
    
    return false;
}

bool MailProvider::matchMX(String * hostname)
{
	return mMxSet->containsObject(hostname->lowercaseString());
}

String * MailProvider::sentMailFolderPath()
{
    return (String *) mMailboxPaths->objectForKey(MCSTR("sentmail"));
}

String * MailProvider::starredFolderPath()
{
	return (String *) mMailboxPaths->objectForKey(MCSTR("starred"));
}

String * MailProvider::allMailFolderPath()
{
	return (String *) mMailboxPaths->objectForKey(MCSTR("allmail"));
}

String * MailProvider::trashFolderPath()
{
	return (String *) mMailboxPaths->objectForKey(MCSTR("trash"));
}

String * MailProvider::draftsFolderPath()
{
	return (String *) mMailboxPaths->objectForKey(MCSTR("drafts"));
}

String * MailProvider::spamFolderPath()
{
	return (String *) mMailboxPaths->objectForKey(MCSTR("spam"));
}

String * MailProvider::importantFolderPath()
{
	return (String *) mMailboxPaths->objectForKey(MCSTR("important"));
}

bool MailProvider::isMainFolder(String * folderPath, String * prefix)
{
	mc_foreachdictionaryValue(String, path, mMailboxPaths) {
		String * fullPath;
        
        if (prefix != NULL) {
            fullPath = prefix->stringByAppendingString((String *) path);
        }
        else {
            fullPath = path;
        }
        
        if (fullPath->isEqual(folderPath))
            return true;
	}
    
    return false;
}

String * MailProvider::description()
{       
    return String::stringWithUTF8Format("<%s:%p, %s>", className()->UTF8Characters(), this, MCUTF8(mIdentifier));
}
