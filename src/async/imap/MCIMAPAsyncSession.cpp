//
//  MCIMAPAsyncSession.cpp
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/17/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPAsyncSession.h"

#include "MCIMAPAsyncConnection.h"
#include "MCIMAPNamespace.h"

#define DEFAULT_MAX_CONNECTIONS 3

using namespace mailcore;

IMAPAsyncSession::IMAPAsyncSession()
{
    mSessions = new Array();
    mMaximumConnections = DEFAULT_MAX_CONNECTIONS;
    mAllowsFolderConcurrentAccessEnabled = true;
}

IMAPAsyncSession::~IMAPAsyncSession()
{
    MC_SAFE_RELEASE(mSessions);
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


void IMAPAsyncSession::setDelimiter(char delimiter)
{
    mDelimiter = delimiter;
}

char IMAPAsyncSession::delimiter()
{
    return mDelimiter;
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

IMAPAsyncConnection * IMAPAsyncSession::session()
{
    IMAPAsyncConnection * session = new IMAPAsyncConnection();
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

IMAPCopyMessagesOperation * IMAPAsyncSession::copyMessagesOperation(String * folder, Array * uids, String * destFolder)
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
                                                                      uint32_t firstUID, uint32_t lastUID)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->fetchMessagesByUIDOperation(folder, requestKind, firstUID, lastUID);
}

IMAPFetchMessagesOperation * IMAPAsyncSession::fetchMessagesByNumberOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                         uint32_t firstNumber, uint32_t lastNumber)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->fetchMessagesByNumberOperation(folder, requestKind, firstNumber, lastNumber);
}

IMAPFetchMessagesOperation * IMAPAsyncSession::fetchMessagesByUIDOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                      Array * uids)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->fetchMessagesByUIDOperation(folder, requestKind, uids);
}

IMAPFetchMessagesOperation * IMAPAsyncSession::fetchMessagesByNumberOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                         Array * numbers)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->fetchMessagesByNumberOperation(folder, requestKind, numbers);
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

IMAPOperation * IMAPAsyncSession::storeFlagsOperation(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, MessageFlag flags)
{
    IMAPAsyncConnection * session = sessionForFolder(folder);
    return session->storeFlagsOperation(folder, uids, kind, flags);
}

IMAPOperation * IMAPAsyncSession::storeLabelsOperation(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, Array * labels)
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

IMAPIdentityOperation * IMAPAsyncSession::identityOperation(String * vendor, String * name, String * version)
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->identityOperation(vendor, name, version);
}

IMAPOperation * IMAPAsyncSession::checkAccountOperation()
{
    IMAPAsyncConnection * session = sessionForFolder(MCSTR("INBOX"));
    return session->checkAccountOperation();
}
