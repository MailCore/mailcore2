//
//  MCOMailProvider.cpp
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCMailProvider.h"
#include "MCNetService.h"

#include <regex.h>

using namespace mailcore;

MailProvider::MailProvider()
{
    init();
}

MailProvider::MailProvider(HashMap * info)
{	
	Array * imapInfos = NULL;
    Array * smtpInfos = NULL;
    Array * popInfos = NULL;
    HashMap * serverInfo = NULL;
	Array * mxs = NULL;
	
    init();
	mDomainMatch = (Array *) info->objectForKey(MCSTR("domain-match"))->retain();
	mMailboxPaths = (HashMap *) info->objectForKey(MCSTR("mailboxes"))->retain();
	mxs = (Array *) info->objectForKey(MCSTR("mx"));
	Enumerator * enumerator = mxs->objectEnumerator();
	String *mxObj = NULL;
	while ((mxObj = (String *) enumerator->nextObject()) != NULL) {
		mMxSet->addObject(mxObj);
	}
	
	serverInfo = (HashMap *) info->objectForKey(MCSTR("servers"));
	imapInfos = (Array *) info->objectForKey(MCSTR("imap"));
	smtpInfos = (Array *) info->objectForKey(MCSTR("smtp"));
	popInfos = (Array *) info->objectForKey(MCSTR("pop"));
	
	AutoreleasePool * pool = new AutoreleasePool();
	
	Enumerator * imapEnumerator = imapInfos->objectEnumerator();
	HashMap * infoObj = NULL;
	while ((infoObj = (HashMap *) imapEnumerator->nextObject()) != NULL) {
		NetService * service = new NetService(infoObj);
        mImapServices->addObject(service);
        service->release();
	}
	
	Enumerator * smtpEnumerator = smtpInfos->objectEnumerator();
	HashMap * smtpInfoObj = NULL;
	while ((smtpInfoObj = (HashMap *) smtpEnumerator->nextObject()) != NULL) {
		NetService * service = new NetService(smtpInfoObj);
        mSmtpServices->addObject(service);
        service->release();
	}

    Enumerator * popEnumerator = popInfos->objectEnumerator();
	HashMap * popInfoObj = NULL;
	while ((popInfoObj = (HashMap *) popEnumerator->nextObject()) != NULL) {
		NetService * service = new NetService(popInfoObj);
        mSmtpServices->addObject(service);
        service->release();
	}
	
	pool->release();
}

void MailProvider::init()
{
	mImapServices = new Array();
    mSmtpServices = new Array();
    mPopServices = new Array();
    mMxSet = new Set();
}

MailProvider::~MailProvider()
{
    MC_SAFE_RELEASE(mMxSet);
	MC_SAFE_RELEASE(mDomainMatch);
    MC_SAFE_RELEASE(mImapServices);
    MC_SAFE_RELEASE(mSmtpServices);
	MC_SAFE_RELEASE(mPopServices);
	MC_SAFE_RELEASE(mMailboxPaths);
	MC_SAFE_RELEASE(mIdentifier);
}

void MailProvider::setIdentifier(String * hostname)
{
    MC_SAFE_REPLACE_COPY(String, mIdentifier, hostname);
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

bool MailProvider::matchEmail(String * email) {
	Array * components;
    String * domain;
    const char * cDomain;
    
    components = email->componentsSeparatedByString(MCSTR("@"));
    if (components->count() < 2)
        return true;
    
    domain = (String *) components->lastObject();
    cDomain = domain->UTF8Characters();

    Enumerator * enumerator = mDomainMatch->objectEnumerator();
	String *match = NULL;
	while ((match = (String *) enumerator->nextObject()) != NULL) {
		regex_t r;
		bool matched;
		
		match = String::stringWithUTF8Format("^%@$", match);
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

bool MailProvider::matchMX(String *hostname)
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
