//
//  MCIMAPAsyncSession.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/17/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPAsyncSession.h"

#include "MCIMAPAsyncConnection.h"
#include "MCIMAPNamespace.h"
#include "MCOperationQueueCallback.h"
#include "MCConnectionLogger.h"
#include "MCIMAPSession.h"
#include "MCIMAPIdentity.h"
#include "MCIMAPMultiDisconnectOperation.h"

#define DEFAULT_MAX_CONNECTIONS 3

using namespace mailcore;

IMAPAsyncSession::IMAPAsyncSession()
{
    mSessions = new Array();
    mMaximumConnections = DEFAULT_MAX_CONNECTIONS;
    mAllowsFolderConcurrentAccessEnabled = true;
    
    mHostname = NULL;
    mPort = 0;
    mUsername = NULL;
    mPassword = NULL;
    mOAuth2Token = NULL;
    mAuthType = AuthTypeSASLNone;
    mConnectionType = ConnectionTypeClear;
    mCheckCertificateEnabled = true;
    mVoIPEnabled = true;
    mDefaultNamespace = NULL;
    mTimeout = 30.;
    mConnectionLogger = NULL;
    mAutomaticConfigurationDone = false;
    mServerIdentity = new IMAPIdentity();
    mClientIdentity = new IMAPIdentity();
    mOperationQueueCallback = NULL;
#if __APPLE__
    mDispatchQueue = dispatch_get_main_queue();
#endif
    mGmailUserDisplayName = NULL;
}

IMAPAsyncSession::~IMAPAsyncSession()
{
    MC_SAFE_RELEASE(mGmailUserDisplayName);
    MC_SAFE_RELEASE(mServerIdentity);
    MC_SAFE_RELEASE(mClientIdentity);
    MC_SAFE_RELEASE(mSessions);
    MC_SAFE_RELEASE(mHostname);
    MC_SAFE_RELEASE(mUsername);
    MC_SAFE_RELEASE(mPassword);
    MC_SAFE_RELEASE(mOAuth2Token);
    MC_SAFE_RELEASE(mDefaultNamespace);
}

void IMAPAsyncSession::setHostname(String * hostname)
{
    MC_SAFE_REPLACE_COPY(String, mHostname, hostname);
}

String * IMAPAsyncSession::hostname()
{
    return mHostname;
}

void IMAPAsyncSession::setPort(unsigned int port)
{
    mPort = port;
}

unsigned int IMAPAsyncSession::port()
{
    return mPort;
}

void IMAPAsyncSession::setUsername(String * username)
{
    MC_SAFE_REPLACE_COPY(String, mUsername, username);
}

String * IMAPAsyncSession::username()
{
    return mUsername;
}

void IMAPAsyncSession::setPassword(String * password)
{
    MC_SAFE_REPLACE_COPY(String, mPassword, password);
}

String * IMAPAsyncSession::password()
{
    return mPassword;
}

void IMAPAsyncSession::setOAuth2Token(String * token)
{
    MC_SAFE_REPLACE_COPY(String, mOAuth2Token, token);
}

String * IMAPAsyncSession::OAuth2Token()
{
    return mOAuth2Token;
}

void IMAPAsyncSession::setAuthType(AuthType authType)
{
    mAuthType = authType;
}

AuthType IMAPAsyncSession::authType()
{
    return mAuthType;
}

void IMAPAsyncSession::setConnectionType(ConnectionType connectionType)
{
    mConnectionType = connectionType;
}

ConnectionType IMAPAsyncSession::connectionType()
{
    return mConnectionType;
}

void IMAPAsyncSession::setTimeout(time_t timeout)
{
    mTimeout = timeout;
}

time_t IMAPAsyncSession::timeout()
{
    return mTimeout;
}

void IMAPAsyncSession::setCheckCertificateEnabled(bool enabled)
{
    mCheckCertificateEnabled = enabled;
}

bool IMAPAsyncSession::isCheckCertificateEnabled()
{
    return mCheckCertificateEnabled;
}

void IMAPAsyncSession::setVoIPEnabled(bool enabled)
{
    mVoIPEnabled = enabled;
}

bool IMAPAsyncSession::isVoIPEnabled()
{
    return mVoIPEnabled;
}

IMAPNamespace * IMAPAsyncSession::defaultNamespace()
{
    return mDefaultNamespace;
}

void IMAPAsyncSession::setDefaultNamespace(IMAPNamespace * ns)
{
    MC_SAFE_REPLACE_RETAIN(IMAPNamespace, mDefaultNamespace, ns);
}

void IMAPAsyncSession::setAllowsFolderConcurrentAccessEnabled(bool enabled)
{
    mAllowsFolderConcurrentAccessEnabled = enabled;
}

bool IMAPAsyncSession::allowsFolderConcurrentAccessEnabled()
{
    return mAllowsFolderConcurrentAccessEnabled;
}

void IMAPAsyncSession::setMaximumConnections(unsigned int maxConnections)
{
    mMaximumConnections = maxConnections;
}

unsigned int IMAPAsyncSession::maximumConnections()
{
    return mMaximumConnections;
}

IMAPIdentity * IMAPAsyncSession::serverIdentity()
{
    return mServerIdentity;
}

IMAPIdentity * IMAPAsyncSession::clientIdentity()
{
    return mClientIdentity;
}

String * IMAPAsyncSession::gmailUserDisplayName()
{
    return mGmailUserDisplayName;
}

IMAPAsyncConnection * IMAPAsyncSession::session()
{
    IMAPAsyncConnection * session = new IMAPAsyncConnection();
    session->setConnectionLogger(mConnectionLogger);
    session->setOwner(this);
    session->autorelease();
    
    session->setHostname(mHostname);
    session->setPort(mPort);
    session->setUsername(mUsername);
    session->setPassword(mPassword);
    session->setOAuth2Token(mOAuth2Token);
    session->setAuthType(mAuthType);
    session->setConnectionType(mConnectionType);
    session->setTimeout(mTimeout);
    session->setCheckCertificateEnabled(mCheckCertificateEnabled);
    session->setVoIPEnabled(mVoIPEnabled);
    session->setDefaultNamespace(mDefaultNamespace);
    session->setClientIdentity(mClientIdentity);
    session->setDispatchQueue(mDispatchQueue);
#if 0 // should be implemented properly
    if (mAutomaticConfigurationDone) {
        session->setAutomaticConfigurationEnabled(false);
    }
#endif
    
    return session;
}

IMAPAsyncConnection * IMAPAsyncSession::sessionForFolder(String * folder, bool urgent)
{
    if (folder == NULL) {
        return availableSession();
    }
    else {
        IMAPAsyncConnection * s = NULL;
        if (urgent && mAllowsFolderConcurrentAccessEnabled) {
            s = availableSession();
            if (s->operationsCount() == 0) {
                s->setLastFolder(folder);
                return s;
            }
        }
        
        s = matchingSessionForFolder(folder);
        s->setLastFolder(folder);
        return s;
    }
}

IMAPAsyncConnection * IMAPAsyncSession::availableSession()
{
    if (mMaximumConnections == 0) {
        for(unsigned int i = 0 ; i < mSessions->count() ; i ++) {
            IMAPAsyncConnection * s = (IMAPAsyncConnection *) mSessions->objectAtIndex(i);
            if (s->operationsCount() == 0)
                return s;
        }
        IMAPAsyncConnection * chosenSession = session();
        mSessions->addObject(chosenSession);
        return chosenSession;
    }
    else {
        IMAPAsyncConnection * chosenSession = NULL;
        unsigned int minOperationsCount = 0;
        for(unsigned int i = 0 ; i < mSessions->count() ; i ++) {
            IMAPAsyncConnection * s = (IMAPAsyncConnection *) mSessions->objectAtIndex(i);
            if (chosenSession == NULL) {
                chosenSession = s;
                minOperationsCount = s->operationsCount();
            }
            else if (s->operationsCount() < minOperationsCount) {
                chosenSession = s;
                minOperationsCount = s->operationsCount();
            }
        }
        if (mSessions->count() < mMaximumConnections) {
            if ((chosenSession != NULL) && (minOperationsCount == 0)) {
                return chosenSession;
            }
            chosenSession = session();
            mSessions->addObject(chosenSession);
            return chosenSession;
        }
        else {
            return chosenSession;
        }
    }
}

IMAPAsyncConnection * IMAPAsyncSession::matchingSessionForFolder(String * folder)
{
    for(unsigned int i = 0 ; i < mSessions->count() ; i ++) {
        IMAPAsyncConnection * currentSession = (IMAPAsyncConnection *) mSessions->objectAtIndex(i);
        if (currentSession->lastFolder() != NULL) {
            if (currentSession->lastFolder()->compare(folder)) {
                return currentSession;
            }
        }
        else {
            return currentSession;
        }
    }
    return availableSession();
}

IMAPFolderInfoOperation * IMAPAsyncSession::folderInfoOperation(String * folder)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->folderInfoOperation(folder);
}

IMAPFolderStatusOperation * IMAPAsyncSession::folderStatusOperation(String * folder)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->folderStatusOperation(folder);
}

IMAPFetchFoldersOperation * IMAPAsyncSession::fetchSubscribedFoldersOperation()
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->fetchSubscribedFoldersOperation();
}

IMAPFetchFoldersOperation * IMAPAsyncSession::fetchAllFoldersOperation()
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->fetchAllFoldersOperation();
}

IMAPOperation * IMAPAsyncSession::renameFolderOperation(String * folder, String * otherName)
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->renameFolderOperation(folder, otherName);
}

IMAPOperation * IMAPAsyncSession::deleteFolderOperation(String * folder)
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->deleteFolderOperation(folder);
}

IMAPOperation * IMAPAsyncSession::createFolderOperation(String * folder)
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->createFolderOperation(folder);
}

IMAPOperation * IMAPAsyncSession::subscribeFolderOperation(String * folder)
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->subscribeFolderOperation(folder);
}

IMAPOperation * IMAPAsyncSession::unsubscribeFolderOperation(String * folder)
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->unsubscribeFolderOperation(folder);
}

IMAPAppendMessageOperation * IMAPAsyncSession::appendMessageOperation(String * folder, Data * messageData, MessageFlag flags)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->appendMessageOperation(folder, messageData, flags);
}

IMAPCopyMessagesOperation * IMAPAsyncSession::copyMessagesOperation(String * folder, IndexSet * uids, String * destFolder)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->copyMessagesOperation(folder, uids, destFolder);
}

IMAPOperation * IMAPAsyncSession::expungeOperation(String * folder)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->expungeOperation(folder);
}

IMAPFetchMessagesOperation * IMAPAsyncSession::fetchMessagesByUIDOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                           IndexSet * uids)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->fetchMessagesByUIDOperation(folder, requestKind, uids);
}

IMAPFetchMessagesOperation * IMAPAsyncSession::fetchMessagesByNumberOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                              IndexSet * numbers)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->fetchMessagesByNumberOperation(folder, requestKind, numbers);
}

IMAPFetchMessagesOperation * IMAPAsyncSession::syncMessagesByUID(String * folder, IMAPMessagesRequestKind requestKind,
                                                                 IndexSet * uids, uint64_t modSeq)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->syncMessagesByUID(folder, requestKind, uids, modSeq);
}

IMAPFetchContentOperation * IMAPAsyncSession::fetchMessageByUIDOperation(String * folder, uint32_t uid, bool urgent)
{
    IMAPAsyncConnection * session = sessionForFolder(folder, urgent);
    return session->fetchMessageByUIDOperation(folder, uid);
}

IMAPFetchContentOperation * IMAPAsyncSession::fetchMessageAttachmentByUIDOperation(String * folder, uint32_t uid, String * partID,
                                                                         Encoding encoding, bool urgent)
{
    IMAPAsyncConnection * session = sessionForFolder(folder, urgent);
    return session->fetchMessageAttachmentByUIDOperation(folder, uid, partID, encoding);
}

IMAPOperation * IMAPAsyncSession::storeFlagsOperation(String * folder, IndexSet * uids, IMAPStoreFlagsRequestKind kind, MessageFlag flags)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->storeFlagsOperation(folder, uids, kind, flags);
}

IMAPOperation * IMAPAsyncSession::storeAllFlagsOperation(String * folder, IndexSet * uids, IMAPStoreFlagsRequestKind kind, MessageFlag flags, Array * customFlags)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->storeAllFlagsOperation(folder, uids, kind, flags, customFlags);
}

IMAPOperation * IMAPAsyncSession::storeLabelsOperation(String * folder, IndexSet * uids, IMAPStoreFlagsRequestKind kind, Array * labels)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->storeLabelsOperation(folder, uids, kind, labels);
}

IMAPSearchOperation * IMAPAsyncSession::searchOperation(String * folder, IMAPSearchKind kind, String * searchString)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->searchOperation(folder, kind, searchString);
}

IMAPSearchOperation * IMAPAsyncSession::searchOperation(String * folder, IMAPSearchExpression * expression)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->searchOperation(folder, expression);
}

IMAPIdleOperation * IMAPAsyncSession::idleOperation(String * folder, uint32_t lastKnownUID)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->idleOperation(folder, lastKnownUID);
}

IMAPFetchNamespaceOperation * IMAPAsyncSession::fetchNamespaceOperation()
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->fetchNamespaceOperation();
}

IMAPIdentityOperation * IMAPAsyncSession::identityOperation(IMAPIdentity * identity)
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->identityOperation(identity);
}

IMAPOperation * IMAPAsyncSession::connectOperation()
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->connectOperation();
}

IMAPOperation * IMAPAsyncSession::checkAccountOperation()
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->checkAccountOperation();
}

IMAPCapabilityOperation * IMAPAsyncSession::capabilityOperation()
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->capabilityOperation();
}

IMAPQuotaOperation * IMAPAsyncSession::quotaOperation()
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->quotaOperation();
}

IMAPOperation * IMAPAsyncSession::noopOperation()
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->noopOperation();
}

IMAPOperation * IMAPAsyncSession::disconnectOperation()
{
    IMAPMultiDisconnectOperation * op = new IMAPMultiDisconnectOperation();
    op->autorelease();
    for(unsigned int i = 0 ; i < mSessions->count() ; i ++) {
        IMAPAsyncConnection * currentSession = (IMAPAsyncConnection *) mSessions->objectAtIndex(i);
        op->addOperation(currentSession->disconnectOperation());
    }
    return op;
}

void IMAPAsyncSession::setConnectionLogger(ConnectionLogger * logger)
{
    mConnectionLogger = logger;
    for(unsigned int i = 0 ; i < mSessions->count() ; i ++) {
        IMAPAsyncConnection * currentSession = (IMAPAsyncConnection *) mSessions->objectAtIndex(i);
        currentSession->setConnectionLogger(logger);
    }
}

ConnectionLogger * IMAPAsyncSession::connectionLogger()
{
    return mConnectionLogger;
}

IMAPMessageRenderingOperation * IMAPAsyncSession::htmlRenderingOperation(IMAPMessage * message,
                                                                         String * folder)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->htmlRenderingOperation(message, folder);
}

IMAPMessageRenderingOperation * IMAPAsyncSession::htmlBodyRenderingOperation(IMAPMessage * message,
                                                                             String * folder)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->htmlBodyRenderingOperation(message, folder);
}

IMAPMessageRenderingOperation * IMAPAsyncSession::plainTextRenderingOperation(IMAPMessage * message,
                                                                              String * folder)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->plainTextRenderingOperation(message, folder);
}

IMAPMessageRenderingOperation * IMAPAsyncSession::plainTextBodyRenderingOperation(IMAPMessage * message,
                                                                                  String * folder,
                                                                                  bool stripWhitespace)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->plainTextBodyRenderingOperation(message, folder, stripWhitespace);
}

void IMAPAsyncSession::automaticConfigurationDone(IMAPSession * session)
{
    MC_SAFE_REPLACE_COPY(IMAPIdentity, mServerIdentity, session->serverIdentity());
    MC_SAFE_REPLACE_COPY(String, mGmailUserDisplayName, session->gmailUserDisplayName());
    setDefaultNamespace(session->defaultNamespace());
    mAutomaticConfigurationDone = true;
}

void IMAPAsyncSession::setOperationQueueCallback(OperationQueueCallback * callback)
{
    mOperationQueueCallback = callback;
}

OperationQueueCallback * IMAPAsyncSession::operationQueueCallback()
{
    return mOperationQueueCallback;
}

bool IMAPAsyncSession::isOperationQueueRunning()
{
    return mQueueRunning;
}

void IMAPAsyncSession::operationRunningStateChanged()
{
    bool isRunning = false;
    for(unsigned int i = 0 ; i < mSessions->count() ; i ++) {
        IMAPAsyncConnection * currentSession = (IMAPAsyncConnection *) mSessions->objectAtIndex(i);
        if (currentSession->isQueueRunning()){
            isRunning = true;
            break;
        }
    }
    if (mQueueRunning == isRunning) {
        return;
    }
    mQueueRunning = isRunning;
    if (mOperationQueueCallback != NULL) {
        if (isRunning) {
            mOperationQueueCallback->queueStartRunning();
        }
        else {
            mOperationQueueCallback->queueStoppedRunning();
        }
    }
}

#if __APPLE__
void IMAPAsyncSession::setDispatchQueue(dispatch_queue_t dispatchQueue)
{
    mDispatchQueue = dispatchQueue;
}

dispatch_queue_t IMAPAsyncSession::dispatchQueue()
{
    return mDispatchQueue;
}
#endif
