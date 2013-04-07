//
//  MCPopAsyncSession.cpp
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include "MCPOPAsyncSession.h"

#include "MCPOP.h"
#include "MCPOPFetchHeaderOperation.h"
#include "MCPOPFetchMessageOperation.h"
#include "MCPOPDeleteMessagesOperation.h"
#include "MCPOPFetchMessagesOperation.h"
#include "MCPOPCheckAccountOperation.h"

using namespace mailcore;

POPAsyncSession::POPAsyncSession()
{
    mSession = new POPSession();
    mQueue = new OperationQueue();
}

POPAsyncSession::~POPAsyncSession()
{
    MC_SAFE_RELEASE(mSession);
    MC_SAFE_RELEASE(mQueue);
}

void POPAsyncSession::setHostname(String * hostname)
{
    mSession->setHostname(hostname);
}

String * POPAsyncSession::hostname()
{
    return mSession->hostname();
}

void POPAsyncSession::setPort(unsigned int port)
{
    mSession->setPort(port);
}

unsigned int POPAsyncSession::port()
{
    return mSession->port();
}

void POPAsyncSession::setUsername(String * username)
{
    mSession->setUsername(username);
}

String * POPAsyncSession::username()
{
    return mSession->username();
}

void POPAsyncSession::setPassword(String * password)
{
    mSession->setPassword(password);
}

String * POPAsyncSession::password()
{
    return mSession->password();
}

void POPAsyncSession::setAuthType(AuthType authType)
{
    mSession->setAuthType(authType);
}

AuthType POPAsyncSession::authType()
{
    return mSession->authType();
}

void POPAsyncSession::setConnectionType(ConnectionType connectionType)
{
    mSession->setConnectionType(connectionType);
}

ConnectionType POPAsyncSession::connectionType()
{
    return mSession->connectionType();
}

void POPAsyncSession::setTimeout(time_t timeout)
{
    mSession->setTimeout(timeout);
}

time_t POPAsyncSession::timeout()
{
    return mSession->timeout();
}

void POPAsyncSession::setCheckCertificateEnabled(bool enabled)
{
    mSession->setCheckCertificateEnabled(enabled);
}

bool POPAsyncSession::isCheckCertificateEnabled()
{
    return mSession->isCheckCertificateEnabled();
}

POPFetchMessagesOperation * POPAsyncSession::fetchMessagesOperation()
{
    POPFetchMessagesOperation * op = new POPFetchMessagesOperation();
    op->setSession(this);
    op->autorelease();
    return op;
}

POPFetchHeaderOperation * POPAsyncSession::fetchHeaderOperation(unsigned int index)
{
    POPFetchHeaderOperation * op = new POPFetchHeaderOperation();
    op->setSession(this);
    op->setMessageIndex(index);
    op->autorelease();
    return op;
}

POPFetchMessageOperation * POPAsyncSession::fetchMessageOperation(unsigned int index)
{
    POPFetchMessageOperation * op = new POPFetchMessageOperation();
    op->setSession(this);
    op->setMessageIndex(index);
    op->autorelease();
    return op;
}

POPOperation * POPAsyncSession::deleteMessagesOperation(IndexSet * indexes)
{
    POPDeleteMessagesOperation * op = new POPDeleteMessagesOperation();
    op->setSession(this);
    op->setMessageIndexes(indexes);
    op->autorelease();
    return op;
}

POPOperation * POPAsyncSession::checkAccountOperation()
{
    POPCheckAccountOperation * op = new POPCheckAccountOperation();
    op->setSession(this);
    op->autorelease();
    return op;
}

POPSession * POPAsyncSession::session()
{
    return mSession;
}

void POPAsyncSession::runOperation(POPOperation * operation)
{
#warning disconnect after delay
    mQueue->addOperation(operation);
}
