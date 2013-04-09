//
//  MasterViewController.m
//  iOS UI Test
//
//  Created by Jonathan Willing on 4/8/13.
//  Copyright (c) 2013 AppJon. All rights reserved.
//

#import "MasterViewController.h"
#import "DetailViewController.h"
#import <MailCore/MailCore.h>
#import "FXKeychain.h"

@interface MasterViewController ()
@property (nonatomic, strong) NSArray *messages;

@property (nonatomic, strong) MCOIMAPOperation *imapCheckOp;
@property (nonatomic, strong) MCOIMAPSession *imapSession;
@property (nonatomic, strong) MCOIMAPFetchMessagesOperation *imapMessagesFetchOp;
@end

@implementation MasterViewController

- (void)viewDidLoad {
    [super viewDidLoad];
	
	NSString *username = [[NSUserDefaults standardUserDefaults] objectForKey:UsernameKey];
	NSString *password = [[FXKeychain defaultKeychain] objectForKey:PasswordKey];
	[self loadAccountWithUsername:username password:password];
}

- (void)loadAccountWithUsername:(NSString *)username password:(NSString *)password {
	if (!username.length || !password.length) {
		[self performSelector:@selector(showSettingsViewController:) withObject:nil afterDelay:0.5];
		return;
	}
	
	self.imapSession = [[MCOIMAPSession alloc] init];
	self.imapSession.hostname = @"imap.gmail.com";
	self.imapSession.port = 993;
	self.imapSession.username = username;
	self.imapSession.password = password;
	self.imapSession.connectionType = MCOConnectionTypeTLS;
	
	NSLog(@"checking account");
	__weak MasterViewController *weakSelf = self;
	self.imapCheckOp = [self.imapSession checkAccountOperation];
	[self.imapCheckOp start:^(NSError *error) {
		MasterViewController *strongSelf = weakSelf;
		NSLog(@"finished checking account.");
		if (error == nil) {
			[strongSelf loadEmails];
		} else {
			NSLog(@"error loading account: %@", error);
		}
		
		strongSelf.imapCheckOp = nil;
	}];
}

- (void)loadEmails {
	MCOIMAPMessagesRequestKind requestKind = (MCOIMAPMessagesRequestKind)
    (MCOIMAPMessagesRequestKindHeaders | MCOIMAPMessagesRequestKindStructure |
     MCOIMAPMessagesRequestKindInternalDate | MCOIMAPMessagesRequestKindHeaderSubject |
     MCOIMAPMessagesRequestKindFlags);
	self.imapMessagesFetchOp = [self.imapSession fetchMessagesByUIDOperationWithFolder:@"INBOX"
																		   requestKind:requestKind
																				  uids:[MCOIndexSet indexSetWithRange:MCORangeMake(1, UINT64_MAX)]];
	[self.imapMessagesFetchOp setProgress:^(unsigned int progress) {
		//NSLog(@"progress: %u", progress);
	}];
	
	__weak MasterViewController *weakSelf = self;
	[self.imapMessagesFetchOp start:^(NSError *error, NSArray *messages, MCOIndexSet *vanishedMessages) {
		MasterViewController *strongSelf = weakSelf;
		NSLog(@"fetched all messages.");
		strongSelf.messages = [NSArray arrayWithArray:messages];
		[strongSelf.tableView reloadData];
	}];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
	NSLog(@"%s",__PRETTY_FUNCTION__);
}

#pragma mark - Table View

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	return self.messages.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"Cell" forIndexPath:indexPath];
	
	MCOIMAPMessage *message = self.messages[indexPath.row];
	cell.textLabel.text = message.header.subject;
	
    return cell;
}

- (void)showSettingsViewController:(id)sender {
	[self.imapMessagesFetchOp cancel];
	
	SettingsViewController *settingsViewController = [[SettingsViewController alloc] initWithNibName:nil bundle:nil];
	settingsViewController.delegate = self;
	[self presentViewController:settingsViewController animated:YES completion:nil];
}

- (void)settingsViewControllerFinished:(SettingsViewController *)viewController {
	[self dismissViewControllerAnimated:YES completion:nil];
	
	NSString *username = [[NSUserDefaults standardUserDefaults] stringForKey:UsernameKey];
	NSString *password = [[FXKeychain defaultKeychain] objectForKey:PasswordKey];
	
	if (![username isEqualToString:self.imapSession.username] || ![password isEqualToString:self.imapSession.password]) {
		self.imapSession = nil;
		[self loadAccountWithUsername:username password:password];
	}
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if ([[segue identifier] isEqualToString:@"showDetail"]) {
        NSIndexPath *indexPath = [self.tableView indexPathForSelectedRow];
        NSDate *object = self.messages[indexPath.row];
        [[segue destinationViewController] setDetailItem:object];
    }
}

@end
