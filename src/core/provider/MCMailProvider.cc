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
	mImapServices = new Array();
    mSmtpServices = new Array();
    mPopServices = new Array();
    mMxSet = new Set();
}

void MailProvider::initWihInfo(HashMap * info)
{
	init();
	
	Array * imapInfos;
    Array * smtpInfos;
    Array * popInfos;
    HashMap * serverInfo;
    Array * mxs;
    	
	mDomainMatch = (Array *) info->objectForKey(MCSTR("domain-match"))->retain();
    mMailboxPaths = (HashMap *) info->objectForKey(MCSTR("mailboxes"))->retain();
    mxs = (Array *) info->objectForKey(MCSTR("mx"));
	mc_foreacharray(String, mx, mxs) {
		mMxSet->addObject(mx->lowercaseString());
	}
    
    serverInfo = (HashMap *) info->objectForKey(MCSTR("servers"));
    imapInfos = (Array *) serverInfo->objectForKey(MCSTR("imap"));
    smtpInfos = (Array *) serverInfo->objectForKey(MCSTR("smtp"));
    popInfos = (Array *) serverInfo->objectForKey(MCSTR("pop"));
    
	mc_foreacharray(HashMap, imapInfo, imapInfos) {
		NetService * service;
        
		service = new NetService(imapInfo);
        mImapServices->addObject(service);
        service->release();
	}
	
	mc_foreacharray(HashMap, smtpInfo, smtpInfos) {
        NetService * service;
        
        service = new NetService(smtpInfo);
		mSmtpServices->addObject(service);
        service->release();
	}
	
	mc_foreacharray(HashMap, popInfo, popInfos) {
        NetService * service;
        
        service = new NetService(popInfo);
		mPopServices->addObject(service);
        service->release();
	}
}

MailProvider::MailProvider(HashMap * info)
{
	initWihInfo(info);
}

MailProvider::MailProvider(Data * infoData) {
	initWihInfo((HashMap *) JSON::objectFromJSONData(infoData));
}

MailProvider::~MailProvider() {
	delete mImapServices;
	delete mSmtpServices;
	delete mPopServices;
	delete mMxSet;
	delete mDomainMatch;
	delete mMailboxPaths;
	delete mIdentifier;
}

void MailProvider::setIdentifier(String * identifier) {
	mIdentifier = identifier;
}

String * MailProvider::identifier() {
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
	mc_foreachdictionaryValue(Object, path, mMailboxPaths) {
		String * fullPath;
        
        if (prefix != NULL) {
            fullPath = prefix->stringByAppendingString((String *) path);
        }
        else {
            fullPath = (String *) path;
        }
        
        if (fullPath->isEqual(folderPath))
            return true;
	}
    
    return false;
}

String * MailProvider::description() {
    return String::stringWithUTF8Format("<%s:%p, %s>", className()->UTF8Characters(), this, identifier());
}
