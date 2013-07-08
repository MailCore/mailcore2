//
//  MasterViewController.m
//  iOS UI Test
//
//  Created by Jonathan Willing on 4/8/13.
//  Copyright (c) 2013 AppJon. All rights reserved.
//

#import "MasterViewController.h"
#import <MailCore/MailCore.h>
#import "FXKeychain.h"
#import "MCTMsgViewController.h"

@interface MasterViewController ()
@property (nonatomic, strong) NSArray *messages;

@property (nonatomic, strong) MCOIMAPOperation *imapCheckOp;
@property (nonatomic, strong) MCOIMAPSession *imapSession;
@property (nonatomic, strong) MCOIMAPFetchMessagesOperation *imapMessagesFetchOp;
@end

@implementation MasterViewController

- (void)viewDidLoad {
	[super viewDidLoad];
	
	[[NSUserDefaults standardUserDefaults] registerDefaults:@{ HostnameKey: @"imap.gmail.com" }];
	
	NSString *username = [[NSUserDefaults standardUserDefaults] objectForKey:UsernameKey];
	NSString *password = [[FXKeychain defaultKeychain] objectForKey:PasswordKey];
	NSString *hostname = [[NSUserDefaults standardUserDefaults] objectForKey:HostnameKey];
	
	[self loadAccountWithUsername:username password:password hostname:hostname];
}

- (void)loadAccountWithUsername:(NSString *)username password:(NSString *)password hostname:(NSString *)hostname {
	if (!username.length || !password.length) {
		[self performSelector:@selector(showSettingsViewController:) withObject:nil afterDelay:0.5];
		return;
	}
	
	self.imapSession = [[MCOIMAPSession alloc] init];
	self.imapSession.hostname = hostname;
	self.imapSession.port = 993;
	self.imapSession.username = username;
	self.imapSession.password = password;
	self.imapSession.connectionType = MCOConnectionTypeTLS;
    MasterViewController * __weak weakSelf = self;
	self.imapSession.connectionLogger = ^(void * connectionID, MCOConnectionLogType type, NSData * data) {
        @synchronized(weakSelf) {
            if (type != MCOConnectionLogTypeSentPrivate) {
                NSLog(@"event logged:%p %i withData: %@", connectionID, type, [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]);
            }
        }
    };
    
	NSLog(@"checking account");
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
		
		NSSortDescriptor *sort = [NSSortDescriptor sortDescriptorWithKey:@"header.date" ascending:NO];
		strongSelf.messages = [messages sortedArrayUsingDescriptors:@[sort]];
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
	NSString *hostname = [[NSUserDefaults standardUserDefaults] objectForKey:HostnameKey];
	
	if (![username isEqualToString:self.imapSession.username] ||
		![password isEqualToString:self.imapSession.password] ||
		![hostname isEqualToString:self.imapSession.hostname]) {
		self.imapSession = nil;
		[self loadAccountWithUsername:username password:password hostname:hostname];
	}
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	MCOIMAPMessage *msg = self.messages[indexPath.row];
	MCTMsgViewController *vc = [[MCTMsgViewController alloc] init];
	vc.folder = @"INBOX";
	vc.message = msg;
	vc.session = self.imapSession;
	[self.navigationController pushViewController:vc animated:YES];
}

@end
