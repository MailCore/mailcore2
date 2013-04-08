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

@interface SettingsViewController ()

@end

@implementation SettingsViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)done:(id)sender {
	[[NSUserDefaults standardUserDefaults] setObject:self.emailTextField.text forKey:UsernameKey];
	[FXKeychain defaultKeychain][PasswordKey] = self.passwordTextField.text;
	
	[self.delegate settingsViewControllerFinished:self];
}

- (void)viewDidLoad {
    [super viewDidLoad];
	
	self.view.backgroundColor = [UIColor underPageBackgroundColor];
	self.emailTextField.text = [[NSUserDefaults standardUserDefaults] stringForKey:UsernameKey];
	self.passwordTextField.text = [FXKeychain defaultKeychain][PasswordKey];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
