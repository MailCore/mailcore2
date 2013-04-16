//
//  MCOMailProvider.m
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOMailProvider.h"

#import "MCOUtils.h"

#import "MCONetService.h"

#include <regex.h>

@implementation MCOMailProvider {
    NSString * _identifier;
    NSArray * _domainMatch;
    NSMutableArray * _imapServices;
    NSMutableArray * _smtpServices;
    NSMutableArray * _popServices;
    NSDictionary * _mailboxPaths;
    NSMutableSet * _mxSet;
}

- (id) init
{
    self = [super init];
    
    _imapServices = [[NSMutableArray alloc] init];
    _smtpServices = [[NSMutableArray alloc] init];
    _popServices = [[NSMutableArray alloc] init];
    _mxSet = [[NSMutableSet alloc] init];
    
    return self;
}

- (void) dealloc
{
    [_mxSet release];
    [_domainMatch release];
    [_imapServices release];
    [_smtpServices release];
    [_popServices release];
    [_mailboxPaths release];
    [_identifier release];
    [super dealloc];
}

- (id) initWithInfo:(NSDictionary *)info
{
    NSArray * imapInfos;
    NSArray * smtpInfos;
    NSArray * popInfos;
    NSDictionary * serverInfo;
    NSArray * mxs;
    
    self = [self init];
    
    _domainMatch = [[info objectForKey:@"domain-match"] retain];
    _mailboxPaths = [[info objectForKey:@"mailboxes"] retain];
    mxs = [info objectForKey:@"mx"];
    for(NSString * mx in mxs) {
        [_mxSet addObject:[mx lowercaseString]];
    }
    
    serverInfo = [info objectForKey:@"servers"];
    imapInfos = [serverInfo objectForKey:@"imap"];
    smtpInfos = [serverInfo objectForKey:@"smtp"];
    popInfos = [serverInfo objectForKey:@"pop"];
    
    for(NSDictionary * info in imapInfos) {
        MCONetService * service;
        
        service = [[MCONetService alloc] initWithInfo:info];
        [_imapServices addObject:service];
        [service release];
    }
    for(NSDictionary * info in smtpInfos) {
        MCONetService * service;
        
        service = [[MCONetService alloc] initWithInfo:info];
        [_smtpServices addObject:service];
        [service release];
    }
    for(NSDictionary * info in popInfos) {
        MCONetService * service;
        
        service = [[MCONetService alloc] initWithInfo:info];
        [_popServices addObject:service];
        [service release];
    }
    
    return self;
}

- (NSArray * /* LEPNetService */) imapServices
{
    return _imapServices;
}

- (NSArray * /* LEPNetService */) smtpServices
{
    return _smtpServices;
}

- (NSArray * /* LEPNetService */) popServices
{
    return _popServices;
}

- (BOOL) matchEmail:(NSString *)email
{
    NSArray * components;
    NSString * domain;
    const char * cDomain;
    
    components = [email componentsSeparatedByString:@"@"];
    if ([components count] < 2)
        return NO;
    
    domain = [components lastObject];
    cDomain = [domain UTF8String];
    for(NSString * match in _domainMatch) {
        regex_t r;
        BOOL matched;
        
        match = [NSString stringWithFormat:@"^%@$", match];
        if (regcomp(&r, [match UTF8String], REG_EXTENDED | REG_ICASE | REG_NOSUB) != 0)
            continue;
        
        matched = NO;
        if (regexec(&r, cDomain, 0, NULL, 0) == 0) {
            matched = YES;
        }
        
        regfree(&r);
        
        if (matched)
            return YES;
    }
    
    return NO;
}

- (BOOL) matchMX:(NSString *)hostname
{
    return [_mxSet containsObject:[hostname lowercaseString]];
}

- (NSString *) sentMailFolderPath
{
    return [_mailboxPaths objectForKey:@"sentmail"];
}

- (NSString *) starredFolderPath
{
    return [_mailboxPaths objectForKey:@"starred"];
}

- (NSString *) allMailFolderPath
{
    return [_mailboxPaths objectForKey:@"allmail"];
}

- (NSString *) trashFolderPath
{
    return [_mailboxPaths objectForKey:@"trash"];
}

- (NSString *) draftsFolderPath
{
    return [_mailboxPaths objectForKey:@"drafts"];
}

- (NSString *) spamFolderPath
{
    return [_mailboxPaths objectForKey:@"spam"];
}

- (NSString *) importantFolderPath
{
    return [_mailboxPaths objectForKey:@"important"];
}

- (BOOL) isMainFolder:(NSString *)folderPath prefix:(NSString *)prefix
{
    for(NSString * path in [_mailboxPaths allValues]) {
        NSString * fullPath;
        
        if (prefix != nil) {
            fullPath = [prefix stringByAppendingString:path];
        }
        else {
            fullPath = path;
        }
        
        if ([fullPath isEqualToString:folderPath])
            return YES;
    }
    
    return NO;
}

@end

