//
//  MCTMsgListViewController.h
//  testUI
//
//  Created by DINH Viêt Hoà on 1/20/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <mailcore/mailcore.h>

@class MCTMsgViewController;

@interface MCTMsgListViewController : NSViewController {
    IBOutlet NSTableView * _tableView;
    IBOutlet MCTMsgViewController * _msgViewController;
    MCOIMAPSession * _session;
    MCOIMAPFetchMessagesOperation * _op;
    NSArray * _messages;
}

@end
