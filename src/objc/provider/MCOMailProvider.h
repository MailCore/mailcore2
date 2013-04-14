//
//  MCOMailProvider.h
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LEPNetService;

@interface MCOMailProvider : NSObject 

@property (nonatomic, copy) NSString * identifier;

- (id) initWithInfo:(NSDictionary *)info;

- (NSArray * /* LEPNetService */) imapServices;
- (NSArray * /* LEPNetService */) smtpServices;
- (NSArray * /* LEPNetService */) popServices;

- (BOOL) matchEmail:(NSString *)email;
- (BOOL) matchMX:(NSString *)hostname;

- (NSString *) sentMailFolderPath;
- (NSString *) starredFolderPath;
- (NSString *) allMailFolderPath;
- (NSString *) trashFolderPath;
- (NSString *) draftsFolderPath;
- (NSString *) spamFolderPath;
- (NSString *) importantFolderPath;

@end
