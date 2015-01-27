//
//  MCAccountValidator.h
//  mailcore2
//
//  Created by Christopher Hockley on 22/01/15.
//  Copyright (c) 2015 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCACCOUNTVALIDATOR_H

#define MAILCORE_MCACCOUNTVALIDATOR_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {
    
    class NetService;
    class MailProvider;
    class IMAPOperation;
    
    class MAILCORE_EXPORT AccountValidator : public Operation, public OperationCallback {
    public:
        AccountValidator();
        virtual ~AccountValidator();
        
        virtual void setEmail(String * email);
        virtual String * email(); /* for SMTP */
        virtual void setUsername(String * username);
        virtual String * username();
        virtual void setPassword(String * password);
        virtual String * password();
        virtual void setOAuth2Token(String * OAuth2Token);
        virtual String * OAuth2Token();
        
        virtual void setImapServices(Array * imapServices);
        virtual Array * /* NetService */ imapServices();
        virtual void setSmtpServices(Array * smtpServices);
        virtual Array * /* NetService */ smtpServices();
        virtual void setPopServices(Array * popServices);
        virtual Array * /* NetService */ popServices();
        
        // result
        virtual String * identifier();
        virtual NetService * imapServer();
        virtual NetService * popServer();
        virtual NetService * smtpServer();
        virtual ErrorCode imapError();
        virtual ErrorCode popError();
        virtual ErrorCode smtpError();
        
        virtual void start();
        
        virtual void setCallback(OperationCallback * callback);
        virtual OperationCallback * Callback();
        
    private:
        String * mEmail; /* for SMTP */
        String * mUsername;
        String * mPassword;
        String * mOAuth2Token;
        
        Array * /* NetService */ mImapServices;
        Array * /* NetService */ mSmtpServices;
        Array * /* NetService */ mPopServices;
        
        // result
        String * mIdentifier;
        NetService * mImapServer;
        NetService * mPopServer;
        NetService * mSmtpServer;
        ErrorCode mImapError;
        ErrorCode mPopError;
        ErrorCode mSmtpError;
        
        void setup();
        void test();
        MailProvider * ResolveProviderUsingMXRecord(String *hostname);
        
        //indexs for services being tested
        int mCurrentServiceIndex;
        int mCurrentServiceTested;
        
        void init();

        OperationCallback * mCallback;
        Operation * mOperation;
        void opCompleted();
        virtual void operationFinished(Operation * op);
    };
}

#endif

#endif
