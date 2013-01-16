//
//  IMAPAsyncSession.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPAsyncSession.h"

#include "MCIMAP.h"
#include "MCIMAPFolderInfoOperation.h"
#include "MCIMAPFetchFoldersOperation.h"
#include "MCIMAPRenameFolderOperation.h"
#include "MCIMAPDeleteFolderOperation.h"
#include "MCIMAPCreateFolderOperation.h"
#include "MCIMAPSubscribeFolderOperation.h"
#include "MCIMAPExpungeOperation.h"
#include "MCIMAPAppendMessageOperation.h"
#include "MCIMAPCopyMessagesOperation.h"
#include "MCIMAPFetchMessagesOperation.h"
#include "MCIMAPFetchContentOperation.h"
#include "MCIMAPFetchContentOperation.h"
#include "MCIMAPStoreFlagsOperation.h"
#include "MCIMAPStoreLabelsOperation.h"
#include "MCIMAPSearchOperation.h"
#include "MCIMAPCheckAccountOperation.h"
#include "MCIMAPFetchNamespaceOperation.h"
#include "MCIMAPIdleOperation.h"
#include "MCIMAPIdentityOperation.h"

using namespace mailcore;

IMAPAsyncSession::IMAPAsyncSession()
{
    mSession = new IMAPSession();
    mQueue = new OperationQueue();
}

IMAPAsyncSession::~IMAPAsyncSession()
{
    MC_SAFE_RELEASE(mDefaultNamespace);
    MC_SAFE_RELEASE(mQueue);
    MC_SAFE_RELEASE(mSession);
}

void IMAPAsyncSession::setHostname(String * hostname)
{
    mSession->setHostname(hostname);
}

String * IMAPAsyncSession::hostname()
{
    return mSession->hostname();
}

void IMAPAsyncSession::setPort(unsigned int port)
{
    mSession->setPort(port);
}

unsigned int IMAPAsyncSession::port()
{
    return mSession->port();
}

void IMAPAsyncSession::setUsername(String * username)
{
    mSession->setUsername(username);
}

String * IMAPAsyncSession::username()
{
    return mSession->username();
}

void IMAPAsyncSession::setPassword(String * password)
{
    mSession->setPassword(password);
}

String * IMAPAsyncSession::password()
{
    return mSession->password();
}

void IMAPAsyncSession::setAuthType(AuthType authType)
{
    mSession->setAuthType(authType);
}

AuthType IMAPAsyncSession::authType()
{
    return mSession->authType();
}

void IMAPAsyncSession::setConnectionType(ConnectionType connectionType)
{
    mSession->setConnectionType(connectionType);
}

ConnectionType IMAPAsyncSession::connectionType()
{
    return mSession->connectionType();
}

void IMAPAsyncSession::setTimeout(time_t timeout)
{
    mSession->setTimeout(timeout);
}

time_t IMAPAsyncSession::timeout()
{
    return mSession->timeout();
}

void IMAPAsyncSession::setCheckCertificateEnabled(bool enabled)
{
    mSession->setCheckCertificateEnabled(enabled);
}

bool IMAPAsyncSession::isCheckCertificateEnabled()
{
    return mSession->isCheckCertificateEnabled();
}

void IMAPAsyncSession::setVoIPEnabled(bool enabled)
{
    mSession->setVoIPEnabled(enabled);
}

bool IMAPAsyncSession::isVoIPEnabled()
{
    return mSession->isVoIPEnabled();
}

void IMAPAsyncSession::setDelimiter(char delimiter)
{
    mSession->setDelimiter(delimiter);
    mDelimiter = delimiter;
}

char IMAPAsyncSession::delimiter()
{
    return mDelimiter;
}

void IMAPAsyncSession::setDefaultNamespace(IMAPNamespace * ns)
{
    mSession->setDefaultNamespace(ns);
    MC_SAFE_REPLACE_RETAIN(IMAPNamespace, mDefaultNamespace, ns);
}

IMAPNamespace * IMAPAsyncSession::defaultNamespace()
{
    return mDefaultNamespace;
}

IMAPFolderInfoOperation * IMAPAsyncSession::folderInfoOperation(String * folder)
{
    IMAPFolderInfoOperation * op = new IMAPFolderInfoOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPFetchFoldersOperation * IMAPAsyncSession::fetchSubscribedFoldersOperation()
{
    IMAPFetchFoldersOperation * op = new IMAPFetchFoldersOperation();
    op->setSession(this);
    op->setFetchSubscribedEnabled(true);
    op->autorelease();
    return op;
}

IMAPFetchFoldersOperation * IMAPAsyncSession::fetchAllFoldersOperation()
{
    IMAPFetchFoldersOperation * op = new IMAPFetchFoldersOperation();
    op->setSession(this);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncSession::renameFolderOperation(String * folder, String * otherName)
{
    IMAPRenameFolderOperation * op = new IMAPRenameFolderOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setOtherName(otherName);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncSession::deleteFolderOperation(String * folder)
{
    IMAPDeleteFolderOperation * op = new IMAPDeleteFolderOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncSession::createFolderOperation(String * folder)
{
    IMAPCreateFolderOperation * op = new IMAPCreateFolderOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncSession::subscribeFolderOperation(String * folder)
{
    IMAPSubscribeFolderOperation * op = new IMAPSubscribeFolderOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncSession::unsubscribeFolderOperation(String * folder)
{
    IMAPSubscribeFolderOperation * op = new IMAPSubscribeFolderOperation();
    op->setSession(this);
    op->setUnsubscribeEnabled(true);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPAppendMessageOperation * IMAPAsyncSession::appendMessageOperation(String * folder, Data * messageData, MessageFlag flags)
{
    IMAPAppendMessageOperation * op = new IMAPAppendMessageOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setMessageData(messageData);
    op->setFlags(flags);
    op->autorelease();
    return op;
}

IMAPCopyMessagesOperation * IMAPAsyncSession::copyMessagesOperation(String * folder, Array * uids, String * destFolder)
{
    IMAPCopyMessagesOperation * op = new IMAPCopyMessagesOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setUids(uids);
    op->setDestFolder(destFolder);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncSession::expungeOperation(String * folder)
{
    IMAPExpungeOperation * op = new IMAPExpungeOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPFetchMessagesOperation * IMAPAsyncSession::fetchMessagesByUIDOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                           uint32_t firstUID, uint32_t lastUID)
{
    IMAPFetchMessagesOperation * op = new IMAPFetchMessagesOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setKind(requestKind);
    op->setFetchByUidEnabled(true);
    op->setFirst(firstUID);
    op->setLast(lastUID);
    op->autorelease();
    return op;
}

IMAPFetchMessagesOperation * IMAPAsyncSession::fetchMessagesByNumberOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                    uint32_t firstNumber, uint32_t lastNumber)
{
    IMAPFetchMessagesOperation * op = new IMAPFetchMessagesOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setKind(requestKind);
    op->setFirst(firstNumber);
    op->setLast(lastNumber);
    op->autorelease();
    return op;
}

IMAPFetchMessagesOperation * IMAPAsyncSession::fetchMessagesByUIDOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                           Array * uids)
{
    IMAPFetchMessagesOperation * op = new IMAPFetchMessagesOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setKind(requestKind);
    op->setFetchByUidEnabled(true);
    op->setUids(uids);
    op->autorelease();
    return op;
}

IMAPFetchMessagesOperation * IMAPAsyncSession::fetchMessagesByNumberOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                              Array * numbers)
{
    IMAPFetchMessagesOperation * op = new IMAPFetchMessagesOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setKind(requestKind);
    op->setNumbers(numbers);
    op->autorelease();
    return op;
}

IMAPFetchContentOperation * IMAPAsyncSession::fetchMessageByUIDOperation(String * folder, uint32_t uid)
{
    IMAPFetchContentOperation * op = new IMAPFetchContentOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setUid(uid);
    op->autorelease();
    return op;
}

IMAPFetchContentOperation * IMAPAsyncSession::fetchMessageAttachmentByUIDOperation(String * folder, uint32_t uid, String * partID,
                                                                                   Encoding encoding, unsigned int expectedSize)
{
    IMAPFetchContentOperation * op = new IMAPFetchContentOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setUid(uid);
    op->setPartID(partID);
    op->setEncoding(encoding);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncSession::storeFlagsOperation(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, MessageFlag flags)
{
    IMAPStoreFlagsOperation * op = new IMAPStoreFlagsOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setUids(uids);
    op->setKind(kind);
    op->setFlags(flags);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncSession::storeLabelsOperation(String * folder, Array * uids, IMAPStoreFlagsRequestKind kind, Array * labels)
{
    IMAPStoreLabelsOperation * op = new IMAPStoreLabelsOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setUids(uids);
    op->setKind(kind);
    op->setLabels(labels);
    op->autorelease();
    return op;
}

IMAPSearchOperation * IMAPAsyncSession::searchOperation(String * folder, IMAPSearchKind kind, String * searchString)
{
    IMAPSearchOperation * op = new IMAPSearchOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setSearchKind(kind);
    op->setSearchString(searchString);
    op->autorelease();
    return op;
}

IMAPSearchOperation * IMAPAsyncSession::searchOperation(String * folder, IMAPSearchExpression * expression)
{
    IMAPSearchOperation * op = new IMAPSearchOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setSearchExpression(expression);
    op->autorelease();
    return op;
}

IMAPIdleOperation * IMAPAsyncSession::idleOperation(String * folder, uint32_t lastKnownUID)
{
    IMAPIdleOperation * op = new IMAPIdleOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setLastKnownUID(lastKnownUID);
    op->autorelease();
    return op;
}

IMAPFetchNamespaceOperation * IMAPAsyncSession::fetchNamespaceOperation()
{
    IMAPFetchNamespaceOperation * op = new IMAPFetchNamespaceOperation();
    op->setSession(this);
    op->autorelease();
    return op;
}

IMAPIdentityOperation * IMAPAsyncSession::identityOperation(String * vendor, String * name, String * version)
{
    IMAPIdentityOperation * op = new IMAPIdentityOperation();
    op->setSession(this);
    op->setVendor(vendor);
    op->setName(name);
    op->setVersion(version);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncSession::checkAccountOperation()
{
    IMAPCheckAccountOperation * op = new IMAPCheckAccountOperation();
    op->setSession(this);
    op->autorelease();
    return op;
}

IMAPSession * IMAPAsyncSession::session()
{
    return mSession;
}

void IMAPAsyncSession::runOperation(IMAPOperation * operation)
{
    // TODO: disconnect after delay
    mQueue->addOperation(operation);
}
