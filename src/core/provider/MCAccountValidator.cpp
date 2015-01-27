//
//  MCAccountValidator.cpp
//  mailcore2
//
//  Created by Christopher Hockley on 22/01/15.
//  Copyright (c) 2015 MailCore. All rights reserved.
//

#include "MCAccountValidator.h"
#include "MCMailProvider.h"
#include "MCMailProvidersManager.h"
#include "MCIMAPAsyncSession.h"
#include "MCPOPAsyncSession.h"
#include "MCSMTPAsyncSession.h"
#include "MCNetService.h"
#include "MCAddress.h"
#include "MCIMAPOperation.h"
#include "MCPOPOperation.h"
#include "MCSMTPOperation.h"

#include <arpa/inet.h>
#include <resolv.h>

using namespace mailcore;

/* this is the service being tested */

enum {
    SERVICE_IMAP,  /* IMAP service */
    SERVICE_POP,   /* POP  service */
    SERVICE_SMTP,  /* SMTP service */
};

void AccountValidator::init()
{
    mEmail = NULL;
    mUsername = NULL;
    mPassword = NULL;
    mOAuth2Token = NULL;
    
    mImapServices = new Array();
    mSmtpServices = new Array();
    mPopServices = new Array();
    
    mIdentifier = NULL;
    mImapServer = NULL;
    mPopServer = NULL;
    mSmtpServer = NULL;
    mImapError = ErrorNone;
    mPopError = ErrorNone;
    mSmtpError = ErrorNone;
    
    mCurrentServiceIndex = 0;
    mCurrentServiceTested = 0;
}

AccountValidator::AccountValidator()
{
    init();
}

AccountValidator::~AccountValidator()
{
    MC_SAFE_RELEASE(mEmail);
    MC_SAFE_RELEASE(mUsername);
    MC_SAFE_RELEASE(mPassword);
    MC_SAFE_RELEASE(mOAuth2Token);
    MC_SAFE_RELEASE(mImapServices);
    MC_SAFE_RELEASE(mSmtpServices);
    MC_SAFE_RELEASE(mPopServices);
    MC_SAFE_RELEASE(mIdentifier);
}

void AccountValidator::start()
{
    setup();
    test();
}

void AccountValidator::operationFinished(Operation * op)
{
    opCompleted();
}

void AccountValidator::setup()
{
    if (mEmail == NULL) {
        if (mUsername == NULL) return;
        else mEmail = mUsername;
    }else if (mUsername == NULL){
        mUsername = mEmail;
    }
    
    MailProvider *provider = MailProvidersManager::sharedManager()->providerForEmail(mUsername);
    
    if (provider == NULL) {
        Array * components;
        String * domain;
    
        components = mUsername->componentsSeparatedByString(MCSTR("@"));
        if (components->count() < 2)
            return;
    
        domain = (String *) components->lastObject();
        provider = ResolveProviderUsingMXRecord(domain);
    }
    
    if (provider != NULL) {
        mIdentifier = provider->identifier();
        
        if (mImapServices->count() == 0 and provider->imapServices()->count() > 0)
            mImapServices = provider->imapServices();
        
        if (mPopServices->count() == 0 and provider->popServices()->count() > 0)
            mPopServices = provider->popServices();
        
        if (mSmtpServices->count() == 0 and provider->smtpServices()->count() > 0)
            mSmtpServices = provider->smtpServices();
    }
    
    if (mImapServices->count() == 0)
        mImapError = ErrorNoValidServerFound;
    
    if (mPopServices->count() == 0)
        mPopError = ErrorNoValidServerFound;
    
    if (mSmtpServices->count() == 0)
        mSmtpError = ErrorNoValidServerFound;
}

void AccountValidator::opCompleted()
{
    ErrorCode error = ErrorNone;
    
    if (mCurrentServiceTested == SERVICE_IMAP) {
        error = mImapError = ((IMAPOperation *)mOperation)->error();
    } else if (mCurrentServiceTested == SERVICE_POP) {
        error = mPopError = ((POPOperation *)mOperation)->error();
    } else if (mCurrentServiceTested == SERVICE_SMTP) {
        error = mSmtpError = ((SMTPOperation *)mOperation)->error();
    }
    
    if (error == ErrorNone) {
        mCurrentServiceTested ++;
    } else {
        mCurrentServiceIndex ++;
    }
    
    test();
}

/**
 Each service(IMAP/POP/SMTP) is tested one after the other.
 For each service we test each server details (NetService),
 Until either:  
    we find on that works and returns ErrorNone in opCompleted().
    we have gone trough the Array of NetService for that service and test() is recalled for the next service.
 */
void AccountValidator::test()
{
    if (mCurrentServiceTested == SERVICE_IMAP) {
        if (mCurrentServiceIndex < mImapServices->count()) {
            IMAPAsyncSession *imapSession = new IMAPAsyncSession();
            imapSession->setUsername(mUsername);
            imapSession->setPassword(mPassword);
        
            mImapServer = (NetService *) mImapServices->objectAtIndex(mCurrentServiceIndex);
            imapSession->setHostname(mImapServer->hostname());
            imapSession->setPort(mImapServer->port());
            imapSession->setConnectionType(mImapServer->connectionType());
            
            mOperation = (IMAPOperation *)imapSession->checkAccountOperation();
            mOperation->setCallback(this);
            mOperation->start();
        
        } else {
            mCurrentServiceTested = SERVICE_POP;
            mCurrentServiceIndex = 0;
            test();
        }
    }else if (mCurrentServiceTested == SERVICE_POP){
        if (mCurrentServiceIndex < mPopServices->count()) {
            POPAsyncSession *popSession = new POPAsyncSession();
            popSession->setUsername(mUsername);
            popSession->setPassword(mPassword);
            
            mPopServer = (NetService *) mPopServices->objectAtIndex(mCurrentServiceIndex);
            popSession->setHostname(mPopServer->hostname());
            popSession->setPort(mPopServer->port());
            popSession->setConnectionType(mPopServer->connectionType());
            
            mOperation = (POPOperation *)popSession->checkAccountOperation();
            mOperation->setCallback(this);
            mOperation->start();
        } else {
            mCurrentServiceTested = SERVICE_SMTP;
            mCurrentServiceIndex = 0;
            test();
        }
    }else if (mCurrentServiceTested == SERVICE_SMTP){
        if (mCurrentServiceIndex < mSmtpServices->count()) {
            SMTPAsyncSession *smtpSession = new SMTPAsyncSession();
            smtpSession->setUsername(mUsername);
            smtpSession->setPassword(mPassword);
            
            mSmtpServer = (NetService *) mSmtpServices->objectAtIndex(mCurrentServiceIndex);
            smtpSession->setHostname(mSmtpServer->hostname());
            smtpSession->setPort(mSmtpServer->port());
            smtpSession->setConnectionType(mSmtpServer->connectionType());
            
            mOperation =  (SMTPOperation *)smtpSession->checkAccountOperation(Address::addressWithMailbox(mEmail));
            mOperation->setCallback(this);
            mOperation->start();

        } else {
           mCallback->operationFinished(this);
        }
    } else {
        mCallback->operationFinished(this);
    }
}

MailProvider* AccountValidator::ResolveProviderUsingMXRecord(String *hostname)
{
    unsigned char response[NS_PACKETSZ];
    ns_msg handle;
    ns_rr rr;
    int len;
    char dispbuf[4096];
    
    if ((len = res_search(MCUTF8(hostname), ns_c_in, ns_t_mx, response, sizeof(response))) < 0) {
        /* WARN: res_search failed */
        return nil;
    }
    
    if (ns_initparse(response, len, &handle) < 0) {
        return nil;
    }
    
    len = ns_msg_count(handle, ns_s_an);
    if (len < 0)
        return nil;
    
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef imageURL = CFBundleCopyResourceURL(mainBundle, CFSTR("providers"), CFSTR("json"), NULL);
    CFStringRef imagePath = CFURLCopyFileSystemPath(imageURL, kCFURLPOSIXPathStyle);
    CFStringEncoding encodingMethod = CFStringGetSystemEncoding();
    const char *path = CFStringGetCStringPtr(imagePath, encodingMethod);
    
    String * sPath = String::stringWithUTF8Characters(path);
    
    MailProvidersManager::sharedManager()->registerProvidersWithFilename(sPath);
    
    for (int ns_index = 0; ns_index < len; ns_index++) {
        if (ns_parserr(&handle, ns_s_an, ns_index, &rr)) {
            /* WARN: ns_parserr failed */
            continue;
        }
        ns_sprintrr (&handle, &rr, NULL, NULL, dispbuf, sizeof (dispbuf));
        if (ns_rr_class(rr) == ns_c_in and ns_rr_type(rr) == ns_t_mx) {
            char mxname[4096];
            dn_expand(ns_msg_base(handle), ns_msg_base(handle) + ns_msg_size(handle), ns_rr_rdata(rr) + NS_INT16SZ, mxname, sizeof(mxname));
            String * str = String::stringWithUTF8Characters(mxname);
            MailProvider *provider = MailProvidersManager::sharedManager()->providerForMX(str);
            if (provider)
                return provider;
        }
    }
    
    return nil;
}


void AccountValidator::setEmail(String * email)
{
    MC_SAFE_REPLACE_COPY(String, mEmail, email);
}

String * AccountValidator::email()
{
    return mEmail;
}

void AccountValidator::setUsername(String * username)
{
    MC_SAFE_REPLACE_COPY(String, mUsername, username);
}

String * AccountValidator::username()
{
    return mUsername;
}

void AccountValidator::setPassword(String * password)
{
    MC_SAFE_REPLACE_COPY(String, mPassword, password);
}

String * AccountValidator::password()
{
    return mPassword;
}

void AccountValidator::setOAuth2Token(String * OAuth2Token)
{
    MC_SAFE_REPLACE_COPY(String, mOAuth2Token, OAuth2Token);
}

String * AccountValidator::OAuth2Token()
{
    return mOAuth2Token;
}

void AccountValidator::setImapServices(Array * imapServices)
{
    MC_SAFE_REPLACE_COPY(Array, mImapServices, imapServices);
}

Array * AccountValidator::imapServices()
{
    return mImapServices;
}

void AccountValidator::setSmtpServices(Array * smtpServices)
{
    MC_SAFE_REPLACE_COPY(Array, mSmtpServices, smtpServices);
}

Array * AccountValidator::smtpServices()
{
    return mSmtpServices;
}

void AccountValidator::setPopServices(Array * popServices)
{
    MC_SAFE_REPLACE_COPY(Array, mPopServices, popServices);
}

Array * AccountValidator::popServices()
{
    return mPopServices;
}

String * AccountValidator::identifier()
{
    return mIdentifier;
}

NetService * AccountValidator::imapServer()
{
    return mImapServer;
}

NetService * AccountValidator::smtpServer()
{
    return mSmtpServer;
}

NetService * AccountValidator::popServer()
{
    return mPopServer;
}

ErrorCode AccountValidator::imapError()
{
    return mImapError;
}

ErrorCode AccountValidator::popError()
{
    return mPopError;
}

ErrorCode AccountValidator::smtpError()
{
    return mSmtpError;
}

void AccountValidator::setCallback(OperationCallback * callback)
{
    mCallback = callback;
}

OperationCallback * AccountValidator::Callback()
{
    return mCallback;
}
