//
//  AppDelegate.m
//  testUI
//
//  Created by DINH Viêt Hoà on 1/19/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "AppDelegate.h"

#import <MailCore/MailCore.h>

#import "MCTMsgListViewController.h"
#import "FXKeychain.h"

@interface AppDelegate () <NSTextFieldDelegate>
@property (nonatomic, copy) NSString *login;
@property (nonatomic, copy) NSString *hostname;
@property (nonatomic, copy) NSString *password;
@property (nonatomic, readonly) BOOL loginEnabled;
@property (nonatomic, readonly) BOOL loggingIn;

@property (nonatomic, retain) MCOIMAPSession *session;
@property (nonatomic, retain) MCOIMAPOperation *checkOp;
@end

@implementation AppDelegate

- (void) dealloc
{
    [super dealloc];
}

- (void) awakeFromNib
{
	[[NSUserDefaults standardUserDefaults] registerDefaults:@{ @"Hostname": @"imap.gmail.com" }];
	
	self.login = [[NSUserDefaults standardUserDefaults] stringForKey:@"Login"];
	self.hostname = [[NSUserDefaults standardUserDefaults] stringForKey:@"Hostname"];
	self.password = [[FXKeychain defaultKeychain] objectForKey:@"Password"];
	
	if (self.login.length && self.password.length) {
		[self accountLogin:nil];
	} else {
		[_accountWindow makeKeyAndOrderFront:nil];
	}
}

- (void) accountLogin:(id)sender
{
    NSLog(@"try login");
	[[NSUserDefaults standardUserDefaults] setObject:self.login forKey:@"Login"];
	[[NSUserDefaults standardUserDefaults] setObject:self.hostname forKey:@"Hostname"];
	
	if (![[[FXKeychain defaultKeychain] objectForKey:@"Password"] isEqualToString:self.password]) {
		[[FXKeychain defaultKeychain] removeObjectForKey:@"Password"];
		[[FXKeychain defaultKeychain] setObject:self.password forKey:@"Password"];
	}	
	
    self.session = [[MCOIMAPSession alloc] init];
    [self.session setHostname:self.hostname];
    [self.session setPort:993];
    [self.session setUsername:self.login];
    [self.session setPassword:self.password];
    [self.session setConnectionType:MCOConnectionTypeTLS];
    self.checkOp = [self.session checkAccountOperation];
	
	NSLog(@"start op");
    [self.checkOp start:^(NSError * error) {
        [_accountWindow orderOut:nil];
	
		[self willChangeValueForKey:@"loggingIn"];
                
        [self.checkOp release];
        _checkOp = nil;
        [self.session release];
        _session = nil;
		
		[self didChangeValueForKey:@"loggingIn"];
		
		NSLog(@"op done (error: %@)", error);
		if (error != nil)
			[_accountWindow makeKeyAndOrderFront:nil];
		
        [_msgListViewController connectWithHostname:self.hostname login:self.login password:self.password];
	}];
}

- (void) accountCancel:(id)sender
{
    [_checkOp cancel];
    [_checkOp release];
    _checkOp = nil;
    [_session release];
    _session = nil;
}

+ (NSSet *)keyPathsForValuesAffectingLoginEnabled
{
	return [NSSet setWithObjects:@"password", @"login", @"hostname", nil];
}

+ (NSSet *)keyPathsForValuesAffectingLoggingIn
{
	return [NSSet setWithObjects:@"checkOp", nil];
}

- (BOOL)loginEnabled
{
	return self.password.length && self.login.length && self.hostname.length && _checkOp == nil;
}

- (BOOL)loggingIn
{
	return (self.checkOp != nil);
}

@end
