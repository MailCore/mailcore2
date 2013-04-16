//
//  MCOMailProvider.h
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

// This class represents a Mail Provider

@interface MCOMailProvider : NSObject 

// This is a dictionary-defined identifier for the current mail provider.
@property (nonatomic, copy) NSString * identifier;

// Returns an initialized mail provider with the provided info dictionary.
// Do not attempt to initialize one of these yourself, let MailCore handle it.
- (id) initWithInfo:(NSDictionary *)info;

// Returns an array of MCONetService objects that represent the IMAP services from this provider.
- (NSArray *) imapServices;

// Returns an array of MCONetService objects that represent the SMTP services from this provider.
- (NSArray *) smtpServices;

// Returns an array of MCONetService objects that represent the POP services from this provider.
- (NSArray *) popServices;

// Returns YES if the given email matches one of the domains offered by this provider, or NO if not.
- (BOOL) matchEmail:(NSString *)email;

// Returns YES if the given MX matches one offered by this provider, or NO if not.
- (BOOL) matchMX:(NSString *)hostname;

// Returns the pre-defined Sent Mail folder path for this provider.
- (NSString *) sentMailFolderPath;

// Returns the pre-defined Starred folder path for this provider.
- (NSString *) starredFolderPath;

// Returns the pre-defined All Mail folder path for this provider.
- (NSString *) allMailFolderPath;

// Returns the pre-defined Trash folder path for this provider.
- (NSString *) trashFolderPath;

// Returns the pre-defined Drafts folder path for this provider.
- (NSString *) draftsFolderPath;

// Returns the pre-defined Spam folder path for this provider.
- (NSString *) spamFolderPath;

// Returns the pre-defined Important folder path for this provider.
- (NSString *) importantFolderPath;

@end
