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

@interface AppDelegate () <NSTextFieldDelegate>

@end

@implementation AppDelegate {
    MCOIMAPSession * _session;
    MCOIMAPOperation * _checkOp;
}

- (void) dealloc
{
    [super dealloc];
}

- (void) awakeFromNib
{
    [_loginTextField setDelegate:self];
    [_passwordTextField setDelegate:self];
}

- (void) applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [self _updateState];
    
    NSString * login = [[NSUserDefaults standardUserDefaults] stringForKey:@"Login"];
    NSString * password = [[NSUserDefaults standardUserDefaults] stringForKey:@"Password"];
    
    if (([login length] == 0) || ([password length] == 0)) {
        [_accountWindow makeKeyAndOrderFront:nil];
    }
    else {
        [_loginTextField setStringValue:login];
        [_passwordTextField setStringValue:password];
    }
}

- (void) accountLogin:(id)sender
{
    NSString * login = [_loginTextField stringValue];
    NSString * password = [_passwordTextField stringValue];
    
    NSLog(@"try login");
    _session = [[MCOIMAPSession alloc] init];
    [_session setHostname:@"imap.gmail.com"];
    [_session setPort:993];
    [_session setUsername:login];
    [_session setPassword:password];
    [_session setConnectionType:MCOConnectionTypeTLS];
    _checkOp = [[_session checkAccountOperation] retain];
    NSLog(@"start op");
    [_checkOp start:^(NSError * error) {
        [_accountWindow orderOut:nil];
        
        NSString * login = [_loginTextField stringValue];
        NSString * password = [_passwordTextField stringValue];
        [[NSUserDefaults standardUserDefaults] setObject:login forKey:@"Login"];
        [[NSUserDefaults standardUserDefaults] setObject:password forKey:@"Password"];
        
        [_checkOp release];
        _checkOp = nil;
        [_session release];
        _session = nil;
        
        [self _updateState];
        
        NSLog(@"op done %@", error);
        
        [_msgListViewController connect];
    }];
    
    [self _updateState];
}

- (void) accountCancel:(id)sender
{
    [_checkOp cancel];
    [_checkOp release];
    _checkOp = nil;
    [_session release];
    _session = nil;
    
    [self _updateState];
}

- (void) _updateState
{
    if (_checkOp == nil) {
        [_loginTextField setEnabled:YES];
        [_passwordTextField setEnabled:YES];
        NSString * login = [_loginTextField stringValue];
        NSString * password = [_passwordTextField stringValue];
        if (([login length] > 0) && ([password length] > 0)) {
            [_loginButton setEnabled:YES];
        }
        else {
            [_loginButton setEnabled:NO];
        }
        [_cancelButton setEnabled:NO];
        [_progressView stopAnimation:nil];
    }
    else {
        [_loginTextField setEnabled:NO];
        [_passwordTextField setEnabled:NO];
        [_loginButton setEnabled:NO];
        [_cancelButton setEnabled:YES];
        [_progressView startAnimation:nil];
    }
}

- (void) controlTextDidChange:(NSNotification *)aNotification
{
    [self _updateState];
}

@end
