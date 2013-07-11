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
#import "GTMOAuth2ViewControllerTouch.h"

#define CLIENT_ID @"the-client-id"
#define CLIENT_SECRET @"the-client-secret"
#define KEYCHAIN_ITEM_NAME @"MailCore OAuth 2.0 Token"

#define NUMBER_OF_MESSAGES_TO_LOAD		10

@interface MasterViewController ()
@property (nonatomic, strong) NSArray *messages;

@property (nonatomic, strong) MCOIMAPOperation *imapCheckOp;
@property (nonatomic, strong) MCOIMAPSession *imapSession;
@property (nonatomic, strong) MCOIMAPFetchMessagesOperation *imapMessagesFetchOp;

@property (nonatomic, strong) NSMutableArray *messageBodyFetchOps;
@property (nonatomic, strong) NSMutableDictionary *messagesBodyDictionary;
@end

@implementation MasterViewController

- (void)viewDidLoad {
	[super viewDidLoad];
	
	[[NSUserDefaults standardUserDefaults] registerDefaults:@{ HostnameKey: @"imap.gmail.com" }];
	
    if ([[NSUserDefaults standardUserDefaults] boolForKey:@"OAuth2Enabled"]) {
        [self startOAuth2];
    }
    else {
        [self startLogin];
    }
}

- (void) startLogin
{
	NSString *username = [[NSUserDefaults standardUserDefaults] objectForKey:UsernameKey];
	NSString *password = [[FXKeychain defaultKeychain] objectForKey:PasswordKey];
	NSString *hostname = [[NSUserDefaults standardUserDefaults] objectForKey:HostnameKey];
    
    if (!username.length || !password.length) {
        [self performSelector:@selector(showSettingsViewController:) withObject:nil afterDelay:0.5];
        return;
    }

	[self loadAccountWithUsername:username password:password hostname:hostname oauth2Token:nil];
}

- (void) startOAuth2
{
    GTMOAuth2Authentication * auth = [GTMOAuth2ViewControllerTouch authForGoogleFromKeychainForName:KEYCHAIN_ITEM_NAME
                                                                                        clientID:CLIENT_ID
                                                                                    clientSecret:CLIENT_SECRET];
    
    if ([auth refreshToken] == nil) {
        MasterViewController * __weak weakSelf = self;
        GTMOAuth2ViewControllerTouch *viewController = [GTMOAuth2ViewControllerTouch controllerWithScope:@"https://mail.google.com/"
                                                                                               clientID:CLIENT_ID
                                                                                           clientSecret:CLIENT_SECRET
                                                                                       keychainItemName:KEYCHAIN_ITEM_NAME
                                                                                       completionHandler:^(GTMOAuth2ViewControllerTouch *viewController, GTMOAuth2Authentication *retrievedAuth, NSError *error) {
                                                                                           [weakSelf loadWithAuth:retrievedAuth];
                                                                                       }];
        [self.navigationController pushViewController:viewController
                                             animated:YES];
    }
    else {
        [auth beginTokenFetchWithDelegate:self
                        didFinishSelector:@selector(auth:finishedRefreshWithFetcher:error:)];
    }
}

- (void)auth:(GTMOAuth2Authentication *)auth
finishedRefreshWithFetcher:(GTMHTTPFetcher *)fetcher
       error:(NSError *)error {
    [self loadWithAuth:auth];
}

- (void)loadWithAuth:(GTMOAuth2Authentication *)auth
{
	NSString *hostname = [[NSUserDefaults standardUserDefaults] objectForKey:HostnameKey];
	[self loadAccountWithUsername:[auth userEmail] password:nil hostname:hostname oauth2Token:[auth accessToken]];
}

- (void)loadAccountWithUsername:(NSString *)username
                       password:(NSString *)password
                       hostname:(NSString *)hostname
                    oauth2Token:(NSString *)oauth2Token
{
	self.imapSession = [[MCOIMAPSession alloc] init];
	self.imapSession.hostname = hostname;
	self.imapSession.port = 993;
	self.imapSession.username = username;
	self.imapSession.password = password;
    if (oauth2Token != nil) {
        self.imapSession.OAuth2Token = oauth2Token;
        self.imapSession.authType = MCOAuthTypeXOAuth2;
    }
	self.imapSession.connectionType = MCOConnectionTypeTLS;
    MasterViewController * __weak weakSelf = self;
	self.imapSession.connectionLogger = ^(void * connectionID, MCOConnectionLogType type, NSData * data) {
        @synchronized(weakSelf) {
            if (type != MCOConnectionLogTypeSentPrivate) {
//                NSLog(@"event logged:%p %i withData: %@", connectionID, type, [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]);
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
	
	MCOIMAPMessagesRequestKind requestKind =
	(MCOIMAPMessagesRequestKind)
	(MCOIMAPMessagesRequestKindHeaders | MCOIMAPMessagesRequestKindStructure |
	 MCOIMAPMessagesRequestKindInternalDate | MCOIMAPMessagesRequestKindHeaderSubject |
	 MCOIMAPMessagesRequestKindFlags);
	
	NSString *inboxFolder = @"INBOX";
	NSUInteger numberOfMessages = NUMBER_OF_MESSAGES_TO_LOAD;
	numberOfMessages--;
	MCOIMAPFolderInfoOperation *inboxFolderInfo = [self.imapSession folderInfoOperation:inboxFolder];
	
	[inboxFolderInfo start:^(NSError *error, MCOIMAPFolderInfo *info) {
		self.imapMessagesFetchOp =
		[self.imapSession fetchMessagesByNumberOperationWithFolder:@"INBOX"
													   requestKind:requestKind
														   numbers:
		 [MCOIndexSet indexSetWithRange:MCORangeMake([info messageCount] - numberOfMessages, numberOfMessages)]];
		
		
		[self.imapMessagesFetchOp setProgress:^(unsigned int progress) {
			NSLog(@"Progress: %u", progress);
		}];
		
		__weak MasterViewController *weakSelf = self;
		[self.imapMessagesFetchOp start:^(NSError *error, NSArray *messages, MCOIndexSet *vanishedMessages) {
			MasterViewController *strongSelf = weakSelf;
			NSLog(@"fetched all messages.");
			
			NSSortDescriptor *sort = [NSSortDescriptor sortDescriptorWithKey:@"header.date" ascending:NO];
			strongSelf.messages = [messages sortedArrayUsingDescriptors:@[sort]];
			[strongSelf loadAllBodiesInMessagesArray:strongSelf.messages];
			[strongSelf.tableView reloadData];
		}];
	}];
	
}

- (void)loadAllBodiesInMessagesArray:(NSArray *)messageArray
{
	NSUInteger messageIdx = 0;
	
	for (MCOIMAPMessage *message in messageArray)
	{
		MCOIMAPFetchContentOperation *op =
		[self.imapSession fetchMessageByUIDOperationWithFolder:@"INBOX" uid:message.uid];
		[self.messageBodyFetchOps addObject:op];
		[op start:^(NSError * error, NSData * data) {
			if ([error code] != MCOErrorNone) {
				return;
			}
			
			NSAssert(data != nil, @"data != nil");
			
			MCOMessageParser *msg = [MCOMessageParser messageParserWithData:data];
			NSString *uidKey = [NSString stringWithFormat:@"%d", message.uid];
			
			self.messagesBodyDictionary[uidKey] = [msg plainTextBodyRendering];
			
			[self.tableView reloadRowsAtIndexPaths:@[[NSIndexPath indexPathForRow:messageIdx inSection:0]]
								  withRowAnimation:UITableViewRowAnimationAutomatic];
			
			[self.messageBodyFetchOps removeObject:op];
		}];
		
		messageIdx++;
	}
}

- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning];
	NSLog(@"%s",__PRETTY_FUNCTION__);
}

#pragma mark -

- (NSMutableArray *)messageBodyFetchOps
{
	if (!_messageBodyFetchOps)
	{
		_messageBodyFetchOps = [[NSMutableArray alloc] initWithCapacity:NUMBER_OF_MESSAGES_TO_LOAD];
	}
	
	return _messageBodyFetchOps;
}

- (NSMutableDictionary *)messagesBodyDictionary
{
	if (!_messagesBodyDictionary)
	{
		_messagesBodyDictionary = [[NSMutableDictionary alloc] initWithCapacity:NUMBER_OF_MESSAGES_TO_LOAD];
	}
	
	return _messagesBodyDictionary;
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
	//	cell.detailTextLabel.text = [NSString stringWithFormat:@"MIME Type: %@", message.mainPart.mimeType];
	NSString *uidKey = [NSString stringWithFormat:@"%d", message.uid];
	cell.detailTextLabel.text = self.messagesBodyDictionary[uidKey];
	
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
		[self loadAccountWithUsername:username password:password hostname:hostname oauth2Token:nil];
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
