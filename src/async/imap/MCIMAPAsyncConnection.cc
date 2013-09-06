//
//  IMAPAsyncConnection.cc
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCIMAPAsyncConnection.h"

#include "MCIMAP.h"
#include "MCIMAPFolderInfoOperation.h"
#include "MCIMAPFolderStatusOperation.h"
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
#include "MCIMAPConnectOperation.h"
#include "MCIMAPCheckAccountOperation.h"
#include "MCIMAPFetchNamespaceOperation.h"
#include "MCIMAPIdleOperation.h"
#include "MCIMAPIdentityOperation.h"
#include "MCIMAPCapabilityOperation.h"
#include "MCIMAPQuotaOperation.h"
#include "MCOperationQueueCallback.h"
#include "MCIMAPDisconnectOperation.h"
#include "MCIMAPAsyncSession.h"
#include "MCConnectionLogger.h"
#include "MCIMAPMessageRenderingOperation.h"
#include "MCIMAPIdentity.h"

using namespace mailcore;

namespace mailcore {
    class IMAPOperationQueueCallback  : public Object, public OperationQueueCallback {
    public:
        IMAPOperationQueueCallback(IMAPAsyncConnection * connection) {
            mConnection = connection;
        }
        
        virtual ~IMAPOperationQueueCallback() {
        }
        
        virtual void queueStartRunning() {
            mConnection->setQueueRunning(true);
            mConnection->owner()->operationRunningStateChanged();
            mConnection->queueStartRunning();
        }
        
        virtual void queueStoppedRunning() {
            mConnection->setQueueRunning(false);
            mConnection->tryAutomaticDisconnect();
            mConnection->owner()->operationRunningStateChanged();
            mConnection->queueStoppedRunning();
        }
        
    private:
        IMAPAsyncConnection * mConnection;
    };
    
    class IMAPConnectionLogger : public Object, public ConnectionLogger {
    public:
        IMAPConnectionLogger(IMAPAsyncConnection * connection) {
            mConnection = connection;
        }
        
        virtual ~IMAPConnectionLogger() {
        }
        
        virtual void log(void * sender, ConnectionLogType logType, Data * buffer)
        {
            mConnection->logConnection(logType, buffer);
        }
        
    private:
        IMAPAsyncConnection * mConnection;
    };
}

IMAPAsyncConnection::IMAPAsyncConnection()
{
    mSession = new IMAPSession();
    mQueue = new OperationQueue();
    mDefaultNamespace = NULL;
    mClientIdentity = new IMAPIdentity();
    mLastFolder = NULL;
    mQueueCallback = new IMAPOperationQueueCallback(this);
    mQueue->setCallback(mQueueCallback);
    mOwner = NULL;
    mConnectionLogger = NULL;
    pthread_mutex_init(&mConnectionLoggerLock, NULL);
    mInternalLogger = new IMAPConnectionLogger(this);
    mAutomaticConfigurationEnabled = true;
    mQueueRunning = false;
}

IMAPAsyncConnection::~IMAPAsyncConnection()
{
    cancelDelayedPerformMethod((Object::Method) &IMAPAsyncConnection::tryAutomaticDisconnectAfterDelay, NULL);
    pthread_mutex_destroy(&mConnectionLoggerLock);
    MC_SAFE_RELEASE(mInternalLogger);
    MC_SAFE_RELEASE(mQueueCallback);
    MC_SAFE_RELEASE(mLastFolder);
    MC_SAFE_RELEASE(mClientIdentity);
    MC_SAFE_RELEASE(mDefaultNamespace);
    MC_SAFE_RELEASE(mQueue);
    MC_SAFE_RELEASE(mSession);
}

void IMAPAsyncConnection::setHostname(String * hostname)
{
    mSession->setHostname(hostname);
}

String * IMAPAsyncConnection::hostname()
{
    return mSession->hostname();
}

void IMAPAsyncConnection::setPort(unsigned int port)
{
    mSession->setPort(port);
}

unsigned int IMAPAsyncConnection::port()
{
    return mSession->port();
}

void IMAPAsyncConnection::setUsername(String * username)
{
    mSession->setUsername(username);
}

String * IMAPAsyncConnection::username()
{
    return mSession->username();
}

void IMAPAsyncConnection::setPassword(String * password)
{
    mSession->setPassword(password);
}

String * IMAPAsyncConnection::password()
{
    return mSession->password();
}

void IMAPAsyncConnection::setOAuth2Token(String * token)
{
    mSession->setOAuth2Token(token);
}

String * IMAPAsyncConnection::OAuth2Token()
{
    return mSession->OAuth2Token();
}

void IMAPAsyncConnection::setAuthType(AuthType authType)
{
    mSession->setAuthType(authType);
}

AuthType IMAPAsyncConnection::authType()
{
    return mSession->authType();
}

void IMAPAsyncConnection::setConnectionType(ConnectionType connectionType)
{
    mSession->setConnectionType(connectionType);
}

ConnectionType IMAPAsyncConnection::connectionType()
{
    return mSession->connectionType();
}

void IMAPAsyncConnection::setTimeout(time_t timeout)
{
    mSession->setTimeout(timeout);
}

time_t IMAPAsyncConnection::timeout()
{
    return mSession->timeout();
}

void IMAPAsyncConnection::setCheckCertificateEnabled(bool enabled)
{
    mSession->setCheckCertificateEnabled(enabled);
}

bool IMAPAsyncConnection::isCheckCertificateEnabled()
{
    return mSession->isCheckCertificateEnabled();
}

void IMAPAsyncConnection::setVoIPEnabled(bool enabled)
{
    mSession->setVoIPEnabled(enabled);
}

bool IMAPAsyncConnection::isVoIPEnabled()
{
    return mSession->isVoIPEnabled();
}

void IMAPAsyncConnection::setDefaultNamespace(IMAPNamespace * ns)
{
    mSession->setDefaultNamespace(ns);
    MC_SAFE_REPLACE_RETAIN(IMAPNamespace, mDefaultNamespace, ns);
}

IMAPNamespace * IMAPAsyncConnection::defaultNamespace()
{
    return mDefaultNamespace;
}

void IMAPAsyncConnection::setClientIdentity(IMAPIdentity * identity)
{
    MC_SAFE_REPLACE_COPY(IMAPIdentity, mClientIdentity, identity);
    mc_foreacharray(String, key, identity->allInfoKeys()) {
        mSession->clientIdentity()->setInfoForKey(key, identity->infoForKey(key));
    }
}

IMAPIdentity * IMAPAsyncConnection::clientIdentity()
{
    return mClientIdentity;
}

IMAPFolderInfoOperation * IMAPAsyncConnection::folderInfoOperation(String * folder)
{
    IMAPFolderInfoOperation * op = new IMAPFolderInfoOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPFolderStatusOperation * IMAPAsyncConnection::folderStatusOperation(String * folder)
{
    IMAPFolderStatusOperation * op = new IMAPFolderStatusOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPFetchFoldersOperation * IMAPAsyncConnection::fetchSubscribedFoldersOperation()
{
    IMAPFetchFoldersOperation * op = new IMAPFetchFoldersOperation();
    op->setSession(this);
    op->setFetchSubscribedEnabled(true);
    op->autorelease();
    return op;
}

IMAPFetchFoldersOperation * IMAPAsyncConnection::fetchAllFoldersOperation()
{
    IMAPFetchFoldersOperation * op = new IMAPFetchFoldersOperation();
    op->setSession(this);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncConnection::renameFolderOperation(String * folder, String * otherName)
{
    IMAPRenameFolderOperation * op = new IMAPRenameFolderOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setOtherName(otherName);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncConnection::deleteFolderOperation(String * folder)
{
    IMAPDeleteFolderOperation * op = new IMAPDeleteFolderOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncConnection::createFolderOperation(String * folder)
{
    IMAPCreateFolderOperation * op = new IMAPCreateFolderOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncConnection::subscribeFolderOperation(String * folder)
{
    IMAPSubscribeFolderOperation * op = new IMAPSubscribeFolderOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncConnection::unsubscribeFolderOperation(String * folder)
{
    IMAPSubscribeFolderOperation * op = new IMAPSubscribeFolderOperation();
    op->setSession(this);
    op->setUnsubscribeEnabled(true);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPAppendMessageOperation * IMAPAsyncConnection::appendMessageOperation(String * folder, Data * messageData, MessageFlag flags)
{
    IMAPAppendMessageOperation * op = new IMAPAppendMessageOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setMessageData(messageData);
    op->setFlags(flags);
    op->autorelease();
    return op;
}

IMAPCopyMessagesOperation * IMAPAsyncConnection::copyMessagesOperation(String * folder, IndexSet * uids, String * destFolder)
{
    IMAPCopyMessagesOperation * op = new IMAPCopyMessagesOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setUids(uids);
    op->setDestFolder(destFolder);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncConnection::expungeOperation(String * folder)
{
    IMAPExpungeOperation * op = new IMAPExpungeOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->autorelease();
    return op;
}

IMAPFetchMessagesOperation * IMAPAsyncConnection::fetchMessagesByUIDOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                              IndexSet * uids)
{
    IMAPFetchMessagesOperation * op = new IMAPFetchMessagesOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setKind(requestKind);
    op->setFetchByUidEnabled(true);
    op->setIndexes(uids);
    op->autorelease();
    return op;
}

IMAPFetchMessagesOperation * IMAPAsyncConnection::fetchMessagesByNumberOperation(String * folder, IMAPMessagesRequestKind requestKind,
                                                                                 IndexSet * numbers)
{
    IMAPFetchMessagesOperation * op = new IMAPFetchMessagesOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setKind(requestKind);
    op->setIndexes(numbers);
    op->autorelease();
    return op;
}

IMAPFetchMessagesOperation * IMAPAsyncConnection::syncMessagesByUID(String * folder, IMAPMessagesRequestKind requestKind,
                                                                    IndexSet * uids, uint64_t modSeq)
{
    IMAPFetchMessagesOperation * op = new IMAPFetchMessagesOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setKind(requestKind);
    op->setFetchByUidEnabled(true);
    op->setIndexes(uids);
    op->setModSequenceValue(modSeq);
    op->autorelease();
    return op;
}

IMAPFetchContentOperation * IMAPAsyncConnection::fetchMessageByUIDOperation(String * folder, uint32_t uid)
{
    IMAPFetchContentOperation * op = new IMAPFetchContentOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setUid(uid);
    op->autorelease();
    return op;
}

IMAPFetchContentOperation * IMAPAsyncConnection::fetchMessageAttachmentByUIDOperation(String * folder, uint32_t uid, String * partID,
                                                                                   Encoding encoding)
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

IMAPOperation * IMAPAsyncConnection::storeFlagsOperation(String * folder, IndexSet * uids, IMAPStoreFlagsRequestKind kind, MessageFlag flags)
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

IMAPOperation * IMAPAsyncConnection::storeLabelsOperation(String * folder, IndexSet * uids, IMAPStoreFlagsRequestKind kind, Array * labels)
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

IMAPSearchOperation * IMAPAsyncConnection::searchOperation(String * folder, IMAPSearchKind kind, String * searchString)
{
    IMAPSearchOperation * op = new IMAPSearchOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setSearchKind(kind);
    op->setSearchString(searchString);
    op->autorelease();
    return op;
}

IMAPSearchOperation * IMAPAsyncConnection::searchOperation(String * folder, IMAPSearchExpression * expression)
{
    IMAPSearchOperation * op = new IMAPSearchOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setSearchExpression(expression);
    op->autorelease();
    return op;
}

IMAPIdleOperation * IMAPAsyncConnection::idleOperation(String * folder, uint32_t lastKnownUID)
{
    IMAPIdleOperation * op = new IMAPIdleOperation();
    op->setSession(this);
    op->setFolder(folder);
    op->setLastKnownUID(lastKnownUID);
    op->autorelease();
    return op;
}

IMAPFetchNamespaceOperation * IMAPAsyncConnection::fetchNamespaceOperation()
{
    IMAPFetchNamespaceOperation * op = new IMAPFetchNamespaceOperation();
    op->setSession(this);
    op->autorelease();
    return op;
}

IMAPIdentityOperation * IMAPAsyncConnection::identityOperation(IMAPIdentity * identity)
{
    IMAPIdentityOperation * op = new IMAPIdentityOperation();
    op->setSession(this);
    op->setClientIdentity(identity);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncConnection::connectOperation()
{
    IMAPConnectOperation * op = new IMAPConnectOperation();
    op->setSession(this);
    op->autorelease();
    return op;
}

IMAPOperation * IMAPAsyncConnection::checkAccountOperation()
{
    IMAPCheckAccountOperation * op = new IMAPCheckAccountOperation();
    op->setSession(this);
    op->autorelease();
    return op;
}

IMAPCapabilityOperation * IMAPAsyncConnection::capabilityOperation()
{
    IMAPCapabilityOperation * op = new IMAPCapabilityOperation();
    op->setSession(this);
    op->autorelease();
    return op;
}

IMAPQuotaOperation * IMAPAsyncConnection::quotaOperation()
{
    IMAPQuotaOperation * op = new IMAPQuotaOperation();
    op->setSession(this);
    op->autorelease();
    return op;
}

IMAPSession * IMAPAsyncConnection::session()
{
    return mSession;
}

unsigned int IMAPAsyncConnection::operationsCount()
{
    return mQueue->count();
}

void IMAPAsyncConnection::runOperation(IMAPOperation * operation)
{
    cancelDelayedPerformMethod((Object::Method) &IMAPAsyncConnection::tryAutomaticDisconnectAfterDelay, NULL);
    mQueue->addOperation(operation);
}

void IMAPAsyncConnection::tryAutomaticDisconnect()
{
    // It's safe since no thread is running when this function is called.
    if (mSession->isDisconnected()) {
        return;
    }
    
    cancelDelayedPerformMethod((Object::Method) &IMAPAsyncConnection::tryAutomaticDisconnectAfterDelay, NULL);
    performMethodAfterDelay((Object::Method) &IMAPAsyncConnection::tryAutomaticDisconnectAfterDelay, NULL, 30);
}

void IMAPAsyncConnection::tryAutomaticDisconnectAfterDelay(void * context)
{
    IMAPDisconnectOperation * op = new IMAPDisconnectOperation();
    op->setSession(this);
    op->autorelease();
    op->start();
}

void IMAPAsyncConnection::queueStartRunning()
{
    this->retain();
    mOwner->retain();
}

void IMAPAsyncConnection::queueStoppedRunning()
{
    mOwner->release();
    this->release();
}

void IMAPAsyncConnection::setLastFolder(String * folder)
{
    MC_SAFE_REPLACE_COPY(String, mLastFolder, folder);
}

String * IMAPAsyncConnection::lastFolder()
{
    return mLastFolder;
}

void IMAPAsyncConnection::setOwner(IMAPAsyncSession * owner)
{
    mOwner = owner;
}

IMAPAsyncSession * IMAPAsyncConnection::owner()
{
    return mOwner;
}

void IMAPAsyncConnection::setConnectionLogger(ConnectionLogger * logger)
{
    pthread_mutex_lock(&mConnectionLoggerLock);
    mConnectionLogger = logger;
    if (mConnectionLogger != NULL) {
        mSession->setConnectionLogger(mInternalLogger);
    }
    else {
        mSession->setConnectionLogger(NULL);
    }
    pthread_mutex_unlock(&mConnectionLoggerLock);
}

ConnectionLogger * IMAPAsyncConnection::connectionLogger()
{
    ConnectionLogger * result;
    
    pthread_mutex_lock(&mConnectionLoggerLock);
    result = mConnectionLogger;
    pthread_mutex_unlock(&mConnectionLoggerLock);
    
    return result;
}

void IMAPAsyncConnection::logConnection(ConnectionLogType logType, Data * buffer)
{
    pthread_mutex_lock(&mConnectionLoggerLock);
    if (mConnectionLogger != NULL) {
        mConnectionLogger->log(this, logType, buffer);
    }
    pthread_mutex_unlock(&mConnectionLoggerLock);
}

IMAPMessageRenderingOperation * IMAPAsyncConnection::renderingOperation(IMAPMessage * message,
                                                                        String * folder,
                                                                        IMAPMessageRenderingType type)
{
    IMAPMessageRenderingOperation * op = new IMAPMessageRenderingOperation();
    op->setSession(this);
    op->setMessage(message);
    op->setFolder(folder);
    op->setRenderingType(type);
    op->autorelease();
    return op;
}

IMAPMessageRenderingOperation * IMAPAsyncConnection::htmlRenderingOperation(IMAPMessage * message,
                                                                            String * folder)
{
    return renderingOperation(message, folder, IMAPMessageRenderingTypeHTML);
}

IMAPMessageRenderingOperation * IMAPAsyncConnection::htmlBodyRenderingOperation(IMAPMessage * message,
                                                                                String * folder)
{
    return renderingOperation(message, folder, IMAPMessageRenderingTypeHTMLBody);
}

IMAPMessageRenderingOperation * IMAPAsyncConnection::plainTextRenderingOperation(IMAPMessage * message,
                                                                                 String * folder)
{
    return renderingOperation(message, folder, IMAPMessageRenderingTypePlainText);
}

IMAPMessageRenderingOperation * IMAPAsyncConnection::plainTextBodyRenderingOperation(IMAPMessage * message,
                                                                                     String * folder)
{
    return renderingOperation(message, folder, IMAPMessageRenderingTypePlainTextBody);
}

void IMAPAsyncConnection::setAutomaticConfigurationEnabled(bool enabled)
{
    mAutomaticConfigurationEnabled = enabled;
    mSession->setAutomaticConfigurationEnabled(enabled);
}

bool IMAPAsyncConnection::isAutomaticConfigurationEnabled()
{
    return mAutomaticConfigurationEnabled;
}

bool IMAPAsyncConnection::isQueueRunning()
{
    return mQueueRunning;
}

void IMAPAsyncConnection::setQueueRunning(bool running)
{
    mQueueRunning = running;
}
