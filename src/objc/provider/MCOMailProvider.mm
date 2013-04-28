//
//  MCOMailProvider.m
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMailProvider.h"
#include "MCMailProvider.h"

#import "NSDictionary+MCO.h"
#import "NSArray+MCO.h"
#import "NSString+MCO.h"

@implementation MCOMailProvider {
	mailcore::MailProvider * _provider;
}

- (mailcore::Object *) mco_mcObject
{
    return _provider;
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::MailProvider * provider = (mailcore::MailProvider *) object;
    return [[self alloc] initWithProvider:provider];
}

- (id) initWithInfo:(NSDictionary *)info
{
	self = [super init];
	
	_provider = new mailcore::MailProvider([info mco_mcHashMap]);
	
	return self;
}

- (id) initWithProvider:(mailcore::MailProvider *)provider
{
	self = [super init];
	
	_provider = provider;
	
	return self;
}

- (NSArray *) imapServices
{
	return [NSArray mco_arrayWithMCArray:_provider->imapServices()];
}

- (NSArray *) smtpServices
{
	return [NSArray mco_arrayWithMCArray:_provider->smtpServices()];
}

- (NSArray *) popServices
{
	return [NSArray mco_arrayWithMCArray:_provider->popServices()];
}

- (BOOL) matchEmail:(NSString *)email
{
	return _provider->matchEmail(email.mco_mcString);
}

- (BOOL) matchMX:(NSString *)hostname
{
	return _provider->matchMX(hostname.mco_mcString);
}

- (NSString *) sentMailFolderPath
{
	return [NSString mco_stringWithMCString:_provider->sentMailFolderPath()];
}

- (NSString *) starredFolderPath
{
	return [NSString mco_stringWithMCString:_provider->starredFolderPath()];
}

- (NSString *) allMailFolderPath
{
	return [NSString mco_stringWithMCString:_provider->allMailFolderPath()];
}

- (NSString *) trashFolderPath
{
	return [NSString mco_stringWithMCString:_provider->trashFolderPath()];
}

- (NSString *) draftsFolderPath
{
	return [NSString mco_stringWithMCString:_provider->draftsFolderPath()];
}

- (NSString *) spamFolderPath
{
	return [NSString mco_stringWithMCString:_provider->spamFolderPath()];
}

- (NSString *) importantFolderPath
{
	return [NSString mco_stringWithMCString:_provider->spamFolderPath()];
}

- (BOOL) isMainFolder:(NSString *)folderPath prefix:(NSString *)prefix {
	return _provider->isMainFolder(folderPath.mco_mcString, prefix.mco_mcString);
}

@end
