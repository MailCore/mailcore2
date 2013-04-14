//
//  MCOMailProvider.m
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCONetService.h"
#include "MCMailProvider.h"

#import "MCOUtils.h"

#import "MCOMailProvider.h"

#include <regex.h>

@implementation MCOMailProvider {
    mailcore::MailProvider * _service;
}

- (id) init
{
    self = [super init];
    
    _service = new mailcore::MailProvider();
	
    return self;
}

- (mailcore::Object *) mco_mcObject
{
    return _service;
}

- (void) dealloc
{
    [super dealloc];
}

- (id) initWithInfo:(NSDictionary *)info
{
	
	_service = new mailcore::MailProvider(info.mco_mcHashMap);

    return self;
}

- (void) setIdentifier:(NSString *)identifier {
	_service->setIdentifier(identifier.mco_mcString);
}

- (NSString *) identifier {
	return [NSString mco_stringWithMCString:_service->identifier()];
}

- (NSArray * /* LEPNetService */) imapServices
{
    return [NSArray mco_arrayWithMCArray:_service->imapServices()];
}

- (NSArray * /* LEPNetService */) smtpServices
{
    return [NSArray mco_arrayWithMCArray:_service->smtpServices()];
}

- (NSArray * /* LEPNetService */) popServices
{
    return [NSArray mco_arrayWithMCArray:_service->popServices()];
}

- (BOOL) matchMX:(NSString *)hostname
{
    return _service->matchMX(hostname.mco_mcString);
}

- (NSString *) sentMailFolderPath
{
    return [NSString mco_stringWithMCString:_service->sentMailFolderPath()];
}

- (NSString *) starredFolderPath
{
    return [NSString mco_stringWithMCString:_service->starredFolderPath()];
}

- (NSString *) allMailFolderPath
{
    return [NSString mco_stringWithMCString:_service->allMailFolderPath()];
}

- (NSString *) trashFolderPath
{
    return [NSString mco_stringWithMCString:_service->trashFolderPath()];
}

- (NSString *) draftsFolderPath
{
    return [NSString mco_stringWithMCString:_service->draftsFolderPath()];
}

- (NSString *) spamFolderPath
{
    return [NSString mco_stringWithMCString:_service->spamFolderPath()];
}

- (NSString *) importantFolderPath
{
    return [NSString mco_stringWithMCString:_service->importantFolderPath()];
}

- (BOOL) matchEmail:(NSString *)email
{
    return _service->matchEmail(email.mco_mcString);
}


@end

