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
    class MXRecordResolverOperation;
    class IMAPAsyncSession;
    class POPAsyncSession;
    class SMTPAsyncSession;

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

        virtual void setImapEnabled(bool enabled);
        virtual bool isImapEnabled();

        virtual void setPopEnabled(bool enabled);
        virtual bool isPopEnabled();

        virtual void setSmtpEnabled(bool enabled);
        virtual bool isSmtpEnabled();

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
        virtual void cancel();
        
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
        
        MailProvider * mProvider;
        
        //indexs for services being tested
        int mCurrentServiceIndex;
        int mCurrentServiceTested;
        
        Operation * mOperation;
        virtual void operationFinished(Operation * op);
        
        OperationQueue * mQueue;
        MXRecordResolverOperation * mResolveMX;
        
        IMAPAsyncSession * mImapSession;
        POPAsyncSession * mPopSession;
        SMTPAsyncSession * mSmtpSession;

        bool mImapEnabled;
        bool mPopEnabled;
        bool mSmtpEnabled;

        void init();
        void setupServices();
        void resolveMX();
        void resolveMXDone();
        void startCheckingHosts();
        void checkNextHost();
        void checkNextHostDone();
    };
}

#endif

#endif
