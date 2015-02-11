//
//  MCOAccountValidator.m
//  mailcore2
//
//  Created by Christopher Hockley on 20/01/15.
//  Copyright (c) 2015 MailCore. All rights reserved.
//

#import "MCOAccountValidator.h"
#include "MCAccountValidator.h"
#include "MCNetService.h"

#import "MCOOperation+Private.h"
#import "MCOUtils.h"
#import "MCOperationCallback.h"
#import "MCONetService.h"

typedef void (^CompletionType)(void);

@interface MCOAccountValidator ()

@property (nonatomic, retain) NSError * imapError;
@property (nonatomic, retain) NSError * popError;
@property (nonatomic, retain) NSError * smtpError;

- (void) operationCompleted;

@end

class MCOValidatorOperationCallback: public mailcore::Object, public mailcore::OperationCallback {
public:
    MCOValidatorOperationCallback(MCOAccountValidator * op)
    {
        mOperation = op;
    }
    
    void operationFinished(mailcore::Operation * op)
    {
        [mOperation operationCompleted];
    }
    
private:
    MCOAccountValidator * mOperation;
};

@implementation MCOAccountValidator{
    CompletionType _completionBlock;
    mailcore::AccountValidator * _validator;
    MCOValidatorOperationCallback * _imapCallback;
}

#define nativeType mailcore::AccountValidator

+ (void) load
{
    MCORegisterClass(self, &typeid(nativeType));
}

- (mailcore::Object *) mco_mcObject
{
    return _validator;
}

+ (NSObject *) mco_objectWithMCObject:(mailcore::Object *)object
{
    mailcore::AccountValidator * validator = (mailcore::AccountValidator *) object;
    return [[[self alloc] initWithMCValidator:validator] autorelease];
}

MCO_OBJC_SYNTHESIZE_STRING(setEmail, email)
MCO_OBJC_SYNTHESIZE_STRING(setUsername, username)
MCO_OBJC_SYNTHESIZE_STRING(setPassword, password)
MCO_OBJC_SYNTHESIZE_STRING(setOAuth2Token, OAuth2Token)

- (id) init
{
    mailcore::AccountValidator * validator = new mailcore::AccountValidator();
    self = [self initWithMCValidator:validator];
    validator->release();
    return self;
}

- (id) initWithMCValidator:(mailcore::AccountValidator *)validator
{
    self = [super initWithMCOperation:validator];
    
    _validator = validator;
    _imapCallback = new MCOValidatorOperationCallback(self);
    _validator->setCallback(_imapCallback);
    _validator->retain();
    
    return self;
}

- (void) start:(void (^)(void))completionBlock
{
    _completionBlock = [completionBlock copy];

    [self start];
}

- (void) cancel
{
    [_completionBlock release];
    _completionBlock = nil;
    [super cancel];
}

- (void) operationCompleted
{
    if (_completionBlock == NULL)
        return;
    
    _completionBlock();
    [_completionBlock release];
    _completionBlock = nil;
}

- (NSString *) identifier
{
    return MCO_OBJC_BRIDGE_GET(identifier);
}

- (MCONetService *) imapServer
{
    return MCO_OBJC_BRIDGE_GET(imapServer);
}

- (MCONetService *) popServer
{
    return MCO_OBJC_BRIDGE_GET(popServer);
}

- (MCONetService *) smtpServer
{
    return MCO_OBJC_BRIDGE_GET(smtpServer);
}

- (NSError *) imapError
{
    return [NSError mco_errorWithErrorCode:_validator->imapError()];
}

- (NSError *) popError
{
    return [NSError mco_errorWithErrorCode:_validator->popError()];
}

- (NSError *) smtpError
{
    return [NSError mco_errorWithErrorCode:_validator->smtpError()];
}

@end
