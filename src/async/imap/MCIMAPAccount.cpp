//
//  MCIMAPAccount.cpp
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/17/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPAccount.h"

#include "MCIMAPAsyncSession.h"
#include "MCIMAPNamespace.h"

#define DEFAULT_MAX_CONNECTIONS 3

using namespace mailcore;

IMAPAccount::IMAPAccount()
{
    mSessions = new Array();
    mMaximumConnections = DEFAULT_MAX_CONNECTIONS;
    mAllowsFolderConcurrentAccessEnabled = true;
}

IMAPAccount::~IMAPAccount()
{
    MC_SAFE_RELEASE(mSessions);
}

void IMAPAccount::setHostname(String * hostname)
{
    MC_SAFE_REPLACE_COPY(String, mHostname, hostname);
}

String * IMAPAccount::hostname()
{
    return mHostname;
}

void IMAPAccount::setPort(unsigned int port)
{
    mPort = port;
}

unsigned int IMAPAccount::port()
{
    return mPort;
}

void IMAPAccount::setUsername(String * username)
{
    MC_SAFE_REPLACE_COPY(String, mUsername, username);
}

String * IMAPAccount::username()
{
    return mUsername;
}

void IMAPAccount::setPassword(String * password)
{
    MC_SAFE_REPLACE_COPY(String, mPassword, password);
}

String * IMAPAccount::password()
{
    return mPassword;
}

void IMAPAccount::setAuthType(AuthType authType)
{
    mAuthType = authType;
}

AuthType IMAPAccount::authType()
{
    return mAuthType;
}

void IMAPAccount::setConnectionType(ConnectionType connectionType)
{
    mConnectionType = connectionType;
}

ConnectionType IMAPAccount::connectionType()
{
    return mConnectionType;
}

void IMAPAccount::setTimeout(time_t timeout)
{
    mTimeout = timeout;
}

time_t IMAPAccount::timeout()
{
    return mTimeout;
}

void IMAPAccount::setCheckCertificateEnabled(bool enabled)
{
    mCheckCertificateEnabled = enabled;
}

bool IMAPAccount::isCheckCertificateEnabled()
{
    return mCheckCertificateEnabled;
}

void IMAPAccount::setVoIPEnabled(bool enabled)
{
    mVoIPEnabled = enabled;
}

bool IMAPAccount::isVoIPEnabled()
{
    return mVoIPEnabled;
}


void IMAPAccount::setDelimiter(char delimiter)
{
    mDelimiter = delimiter;
}

char IMAPAccount::delimiter()
{
    return mDelimiter;
}

IMAPNamespace * IMAPAccount::defaultNamespace()
{
    return mDefaultNamespace;
}

void IMAPAccount::setDefaultNamespace(IMAPNamespace * ns)
{
    MC_SAFE_REPLACE_RETAIN(IMAPNamespace, mDefaultNamespace, ns);
}

void IMAPAccount::setAllowsFolderConcurrentAccessEnabled(bool enabled)
{
    mAllowsFolderConcurrentAccessEnabled = enabled;
}

bool IMAPAccount::allowsFolderConcurrentAccessEnabled()
{
    return mAllowsFolderConcurrentAccessEnabled;
}

void IMAPAccount::setMaximumConnections(unsigned int maxConnections)
{
    mMaximumConnections = maxConnections;
}

unsigned int IMAPAccount::maximumConnections()
{
    return mMaximumConnections;
}

IMAPAsyncSession * IMAPAccount::session()
{
    IMAPAsyncSession * session = new IMAPAsyncSession();
    session->autorelease();
    
    session->setHostname(mHostname);
    session->setPort(mPort);
    session->setUsername(mUsername);
    session->setPassword(mPassword);
    session->setAuthType(mAuthType);
    session->setConnectionType(mConnectionType);
    session->setTimeout(mTimeout);
    session->setCheckCertificateEnabled(mCheckCertificateEnabled);
    session->setVoIPEnabled(mVoIPEnabled);
    session->setDelimiter(mDelimiter);
    session->setDefaultNamespace(mDefaultNamespace);
    
    return session;
}

IMAPAsyncSession * IMAPAccount::sessionForFolder(String * folder, bool urgent)
{
    if (folder == NULL) {
        return availableSession();
    }
    else {
        IMAPAsyncSession * s = NULL;
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

IMAPAsyncSession * IMAPAccount::availableSession()
{
    if (mMaximumConnections == 0) {
        for(unsigned int i = 0 ; i < mSessions->count() ; i ++) {
            IMAPAsyncSession * s = (IMAPAsyncSession *) mSessions->objectAtIndex(i);
            if (s->operationsCount() == 0)
                return s;
        }
        IMAPAsyncSession * chosenSession = session();
        mSessions->addObject(chosenSession);
        return chosenSession;
    }
    else {
        IMAPAsyncSession * chosenSession = NULL;
        unsigned int minOperationsCount = 0;
        for(unsigned int i = 0 ; i < mSessions->count() ; i ++) {
            IMAPAsyncSession * s = (IMAPAsyncSession *) mSessions->objectAtIndex(i);
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

IMAPAsyncSession * IMAPAccount::matchingSessionForFolder(String * folder)
{
    for(unsigned int i = 0 ; i < mSessions->count() ; i ++) {
        IMAPAsyncSession * currentSession = (IMAPAsyncSession *) mSessions->objectAtIndex(i);
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

IMAPFolderInfoOperation * IMAPAccount::folderInfoOperation(String * folder)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->folderInfoOperation(folder);
}

IMAPFetchFoldersOperation * IMAPAccount::fetchSubscribedFoldersOperation()
{
    IMAPAsyncSession * session = sessionForFolder(MCSTR("INBOX"));
    return session->fetchSubscribedFoldersOperation();
}

IMAPFetchFoldersOperation * IMAPAccount::fetchAllFoldersOperation()
{
    IMAPAsyncSession * session = sessionForFolder(MCSTR("INBOX"));
    return session->fetchAllFoldersOperation();
}

IMAPOperation * IMAPAccount::renameFolderOperation(String * folder, String * otherName)
{
    IMAPAsyncSession * session = sessionForFolder(MCSTR("INBOX"));
    return session->renameFolderOperation(folder, otherName);
}

IMAPOperation * IMAPAccount::deleteFolderOperation(String * folder)
{
    IMAPAsyncSession * session = sessionForFolder(MCSTR("INBOX"));
    return session->deleteFolderOperation(folder);
}

IMAPOperation * IMAPAccount::createFolderOperation(String * folder)
{
    IMAPAsyncSession * session = sessionForFolder(MCSTR("INBOX"));
    return session->createFolderOperation(folder);
}

IMAPOperation * IMAPAccount::subscribeFolderOperation(String * folder)
{
    IMAPAsyncSession * session = sessionForFolder(MCSTR("INBOX"));
    return session->subscribeFolderOperation(folder);
}

IMAPOperation * IMAPAccount::unsubscribeFolderOperation(String * folder)
{
    IMAPAsyncSession * session = sessionForFolder(MCSTR("INBOX"));
    return session->unsubscribeFolderOperation(folder);
}

IMAPAppendMessageOperation * IMAPAccount::appendMessageOperation(String * folder, Data * messageData, MessageFlag flags)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->appendMessageOperation(folder, messageData, flags);
}

IMAPCopyMessagesOperation * IMAPAccount::copyMessagesOperation(String * folder, Array * uids, String * destFolder)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->copyMessagesOperation(folder, uids, destFolder);
}

IMAPOperation * IMAPAccount::expungeOperation(String * folder)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->expungeOperation(folder);
}

IMAPFetchMessagesOperation * IMAPAccount::fetchMessagesByUIDOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                      uint32_t firstUID, uint32_t lastUID)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->fetchMessagesByUIDOperation(folder, requestKind, firstUID, lastUID);
}

IMAPFetchMessagesOperation * IMAPAccount::fetchMessagesByNumberOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                         uint32_t firstNumber, uint32_t lastNumber)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->fetchMessagesByNumberOperation(folder, requestKind, firstNumber, lastNumber);
}

IMAPFetchMessagesOperation * IMAPAccount::fetchMessagesByUIDOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                      Array * uids)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->fetchMessagesByUIDOperation(folder, requestKind, uids);
}

IMAPFetchMessagesOperation * IMAPAccount::fetchMessagesByNumberOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                         Array * numbers)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->fetchMessagesByNumberOperation(folder, requestKind, numbers);
}

IMAPFetchContentOperation * IMAPAccount::fetchMessageByUIDOperation(String * folder, uint32_t uid, bool urgent)
{
    IMAPAsyncSession * session = sessionForFolder(folder, urgent);
    return session->fetchMessageByUIDOperation(folder, uid);
}

IMAPFetchContentOperation * IMAPAccount::fetchMessageAttachmentByUIDOperation(String * folder, uint32_t uid, String * partID,
                                                                         Encoding encoding, bool urgent)
{
    IMAPAsyncSession * session = sessionForFolder(folder, urgent);
    return session->fetchMessageAttachmentByUIDOperation(folder, uid, partID, encoding);
}

IMAPOperation * IMAPAccount::storeFlagsOperation(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, MessageFlag flags)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->storeFlagsOperation(folder, uids, kind, flags);
}

IMAPOperation * IMAPAccount::storeLabelsOperation(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, Array * labels)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->storeLabelsOperation(folder, uids, kind, labels);
}

IMAPSearchOperation * IMAPAccount::searchOperation(String * folder, IMAPSearchKind kind, String * searchString)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->searchOperation(folder, kind, searchString);
}

IMAPSearchOperation * IMAPAccount::searchOperation(String * folder, IMAPSearchExpression * expression)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->searchOperation(folder, expression);
}

IMAPIdleOperation * IMAPAccount::idleOperation(String * folder, uint32_t lastKnownUID)
{
    IMAPAsyncSession * session = sessionForFolder(folder);
    return session->idleOperation(folder, lastKnownUID);
}

IMAPFetchNamespaceOperation * IMAPAccount::fetchNamespaceOperation()
{
    IMAPAsyncSession * session = sessionForFolder(MCSTR("INBOX"));
    return session->fetchNamespaceOperation();
}

IMAPIdentityOperation * IMAPAccount::identityOperation(String * vendor, String * name, String * version)
{
    IMAPAsyncSession * session = sessionForFolder(MCSTR("INBOX"));
    return session->identityOperation(vendor, name, version);
}

IMAPOperation * IMAPAccount::checkAccountOperation()
{
    IMAPAsyncSession * session = sessionForFolder(MCSTR("INBOX"));
    return session->checkAccountOperation();
}
