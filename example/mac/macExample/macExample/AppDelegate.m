//
//  AppDelegate.m
//  testUI
//
//  Created by DINH Viêt Hoà on 1/19/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "AppDelegate.h"

#import <MailCore/MailCore.h>
#import "GTMOAuth2WindowController.h"
#import "GTMOAuth2SignIn.h"

#import "MCTMsgListViewController.h"
#import "FXKeychain.h"

@interface AppDelegate () <NSTextFieldDelegate>
@property (nonatomic, copy) NSString *login;
@property (nonatomic, copy) NSString *hostname;
@property (nonatomic, copy) NSString *password;
@property (nonatomic, copy) NSString *oauth2Token;
@property (nonatomic, readonly) BOOL loginEnabled;
@property (nonatomic, readonly) BOOL loggingIn;

@property (nonatomic, retain) MCOIMAPSession *session;
@property (nonatomic, retain) MCOIMAPOperation *checkOp;
@end

#define CLIENT_ID @"the-client-id"
#define CLIENT_SECRET @"the-client-secret"
#define KEYCHAIN_ITEM_NAME @"MailCore OAuth 2.0 Token"

@implementation AppDelegate

- (void) dealloc
{
    self.login = nil;
    self.hostname = nil;
    self.password = nil;
    self.oauth2Token = nil;
    self.session = nil;
    self.checkOp = nil;
    [super dealloc];
}

- (void) awakeFromNib
{
	[[NSUserDefaults standardUserDefaults] registerDefaults:@{ @"Hostname": @"imap.gmail.com" }];
	self.hostname = [[NSUserDefaults standardUserDefaults] stringForKey:@"Hostname"];
    
    if ([[NSUserDefaults standardUserDefaults] boolForKey:@"OAuth2Enabled"]) {
        [self startOAuth2];
    }
    else {
        [self startLogin];
    }
}

- (void) startOAuth2
{
    GTMOAuth2Authentication * auth = [GTMOAuth2WindowController authForGoogleFromKeychainForName:KEYCHAIN_ITEM_NAME
                                                                                        clientID:CLIENT_ID
                                                                                    clientSecret:CLIENT_SECRET];
    
    if ([auth refreshToken] == nil) {
        GTMOAuth2WindowController *windowController = [[[GTMOAuth2WindowController alloc] initWithScope:@"https://mail.google.com/"
                                                                                               clientID:CLIENT_ID
                                                                                           clientSecret:CLIENT_SECRET
                                                                                       keychainItemName:KEYCHAIN_ITEM_NAME
                                                                                         resourceBundle:[NSBundle bundleForClass:[GTMOAuth2WindowController class]]] autorelease];
        [windowController signInSheetModalForWindow:nil
                                           delegate:self
                                   finishedSelector:@selector(windowController:finishedWithAuth:error:)];
    }
    else {
        [auth beginTokenFetchWithDelegate:self
                        didFinishSelector:@selector(auth:finishedRefreshWithFetcher:error:)];
    }
}

- (void)auth:(GTMOAuth2Authentication *)auth
finishedRefreshWithFetcher:(GTMHTTPFetcher *)fetcher
       error:(NSError *)error {
    [self windowController:nil finishedWithAuth:auth error:error];
}

- (void)windowController:(GTMOAuth2WindowController *)viewController
        finishedWithAuth:(GTMOAuth2Authentication *)auth
                   error:(NSError *)error
{
    if (error != nil) {
        NSLog(@"request failed");
        // Authentication failed
    } else {
        // Authentication succeeded
        [self loginWithAuth:auth];
    }
}

- (void) loginWithAuth:(GTMOAuth2Authentication *)auth
{
    self.login = [auth userEmail];
    self.oauth2Token = [auth accessToken];
    
    [self accountLogin:nil];
}

- (void) startLogin
{
	self.login = [[NSUserDefaults standardUserDefaults] stringForKey:@"Login"];
	self.password = [[FXKeychain defaultKeychain] objectForKey:@"Password"];
	if (self.login.length && self.password.length) {
		[self accountLogin:nil];
	} else {
		[_accountWindow makeKeyAndOrderFront:nil];
	}
}

- (void) accountLogin:(id)sender
{
    self.session = [[[MCOIMAPSession alloc] init] autorelease];
    [self.session setHostname:self.hostname];
    [self.session setPort:993];
    
    NSLog(@"try login");
    if (self.oauth2Token != nil) {
        [self.session setUsername:self.login];
        [self.session setOAuth2Token:self.oauth2Token];
        [self.session setAuthType:MCOAuthTypeXOAuth2];
    }
    else {
        [[NSUserDefaults standardUserDefaults] setObject:self.login forKey:@"Login"];
        [[NSUserDefaults standardUserDefaults] setObject:self.hostname forKey:@"Hostname"];
        
        if (![[[FXKeychain defaultKeychain] objectForKey:@"Password"] isEqualToString:self.password]) {
            [[FXKeychain defaultKeychain] removeObjectForKey:@"Password"];
            [[FXKeychain defaultKeychain] setObject:self.password forKey:@"Password"];
        }
        
        [self.session setUsername:self.login];
        [self.session setPassword:self.password];
    }
	
    [self.session setConnectionType:MCOConnectionTypeTLS];
    self.checkOp = [self.session checkAccountOperation];
	self.session.connectionLogger = ^(void * connectionID, MCOConnectionLogType type, NSData * data) {
        if (type != MCOConnectionLogTypeSentPrivate) {
            NSLog(@"event logged:%p %i withData: %@", connectionID, (int) type, [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease]);
        }
    };
	
	NSLog(@"start op");
    [self.checkOp start:^(NSError * error) {
        [_accountWindow orderOut:nil];
	
		[self willChangeValueForKey:@"loggingIn"];
                
        self.checkOp = nil;
        self.session = nil;
		
		[self didChangeValueForKey:@"loggingIn"];
		
		NSLog(@"op done (error: %@)", error);
		if (error != nil)
			[_accountWindow makeKeyAndOrderFront:nil];
		
        [_msgListViewController connectWithHostname:self.hostname login:self.login password:self.password oauth2Token:self.oauth2Token];
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
