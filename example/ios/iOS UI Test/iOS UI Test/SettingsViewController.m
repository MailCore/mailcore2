//
//  SettingsViewController.m
//  iOS UI Test
//
//  Created by Jonathan Willing on 4/8/13.
//  Copyright (c) 2013 AppJon. All rights reserved.
//

#import "SettingsViewController.h"
#import "FXKeychain.h"

NSString * const UsernameKey = @"username";
NSString * const PasswordKey = @"password";
NSString * const HostnameKey = @"hostname";
NSString * const FetchFullMessageKey = @"FetchFullMessageEnabled";

@implementation SettingsViewController

- (void)done:(id)sender {
	[[NSUserDefaults standardUserDefaults] setObject:self.emailTextField.text ?: @"" forKey:UsernameKey];
	[[FXKeychain defaultKeychain] setObject:self.passwordTextField.text ?: @"" forKey:PasswordKey];
	[[NSUserDefaults standardUserDefaults] setObject:self.hostnameTextField.text ?: @"" forKey:HostnameKey];
    [[NSUserDefaults standardUserDefaults] setBool:[self.fetchFullMessageSwitch isOn] forKey:FetchFullMessageKey];
	
	[self.delegate settingsViewControllerFinished:self];
}

- (void)viewDidLoad {
    [super viewDidLoad];
	
	self.view.backgroundColor = [UIColor underPageBackgroundColor];
	self.emailTextField.text = [[NSUserDefaults standardUserDefaults] stringForKey:UsernameKey];
	self.passwordTextField.text = [[FXKeychain defaultKeychain] objectForKey:PasswordKey];
	self.hostnameTextField.text = [[NSUserDefaults standardUserDefaults] stringForKey:HostnameKey];
    self.fetchFullMessageSwitch.on = [[NSUserDefaults standardUserDefaults] boolForKey:FetchFullMessageKey];
}

@end
