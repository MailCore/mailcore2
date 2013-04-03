//
//  MCTMsgListViewController.m
//  testUI
//
//  Created by DINH Viêt Hoà on 1/20/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCTMsgListViewController.h"

#include <MailCore/MailCore.h>

#import "MCTMsgViewController.h"

#define FOLDER @"INBOX"

@interface MCTMsgListViewController () <NSTableViewDelegate, NSTableViewDataSource>

@end

@implementation MCTMsgListViewController

- (void) awakeFromNib
{
    [self connect];
}

- (void) connect
{
    [_msgViewController setFolder:FOLDER];
    
    NSString * login = [[NSUserDefaults standardUserDefaults] stringForKey:@"Login"];
    NSString * password = [[NSUserDefaults standardUserDefaults] stringForKey:@"Password"];
    if (([login length] == 0) || ([password length] == 0))
        return;
    
    _session = [[MCOIMAPSession alloc] init];
    [_session setHostname:@"imap.gmail.com"];
    [_session setPort:993];
    [_session setUsername:login];
    [_session setPassword:password];
    [_session setConnectionType:MCOConnectionTypeTLS];
    
    MCOIMAPMessagesRequestKind requestKind = (MCOIMAPMessagesRequestKind)
    (MCOIMAPMessagesRequestKindHeaders | MCOIMAPMessagesRequestKindStructure |
     MCOIMAPMessagesRequestKindInternalDate | MCOIMAPMessagesRequestKindHeaderSubject |
     MCOIMAPMessagesRequestKindFlags);
    
    _op = [_session fetchMessagesByUIDOperationWithFolder:FOLDER requestKind:requestKind uids:[MCOIndexSet indexSetWithRange:MCORangeMake(1, UINT64_MAX)]];
    [_op setProgress:^(unsigned int current){
        //NSLog(@"progress: %u", current);
    }];
    [_op start:^(NSError * error, NSArray * messages, MCOIndexSet * vanishedMessages) {
        [_messages release];
        _messages = [messages retain];
        NSLog(@"%i messages", (int) [_messages count]);
        //NSLog(@"%@", _messages);
        [_tableView reloadData];
    }];
}

- (NSInteger) numberOfRowsInTableView:(NSTableView *)aTableView
{
    return [_messages count];
}

- (id) tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    MCOIMAPMessage * msg = [_messages objectAtIndex:rowIndex];
    return [[msg header] subject];
}

- (void) tableViewSelectionDidChange:(NSNotification *)notification
{
    int rowIndex = (int) [_tableView selectedRow];
    if (rowIndex == -1) {
        [_msgViewController setSession:nil];
        [_msgViewController setMessage:nil];
    }
    else {
        MCOIMAPMessage * msg = [_messages objectAtIndex:rowIndex];
        [_msgViewController setSession:_session];
        [_msgViewController setMessage:msg];
    }
}

@end
