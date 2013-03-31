//
//  MCOIMAPBaseOperation.m
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/26/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import "MCOIMAPBaseOperation.h"

#import "MCOOperation+Private.h"

#import "MCAsyncIMAP.h"

class MCOIMAPBaseOperationIMAPCallback : public mailcore::IMAPOperationCallback {
public:
    MCOIMAPBaseOperationIMAPCallback(MCOIMAPBaseOperation * op)
    {
        mOperation = op;
    }
    
    virtual void bodyProgress(mailcore::IMAPOperation * session, unsigned int current, unsigned int maximum) {
        [mOperation bodyProgress:current maximum:maximum];
    }
    
    virtual void itemProgress(mailcore::IMAPOperation * session, unsigned int current, unsigned int maximum) {
        [mOperation itemProgress:current maximum:maximum];
    }
    
private:
    MCOIMAPBaseOperation * mOperation;
};

@implementation MCOIMAPBaseOperation {
    MCOIMAPBaseOperationIMAPCallback * _imapCallback;
}

- (id) initWithMCOperation:(mailcore::Operation *)op
{
    self = [super initWithMCOperation:op];
    
    _imapCallback = new MCOIMAPBaseOperationIMAPCallback(self);
    ((mailcore::IMAPOperation *) op)->setImapCallback(_imapCallback);
    
    return self;
}

- (void) dealloc
{
    delete _imapCallback;
    [super dealloc];
}

- (void) bodyProgress:(unsigned int)current maximum:(unsigned int)maximum
{
}

- (void) itemProgress:(unsigned int)current maximum:(unsigned int)maximum
{
}

@end
