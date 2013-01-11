#include "MCSMTPAsyncSession.h"

#include "MCSMTPSession.h"
#include "MCSMTPSendWithRecipientOperation.h"
#include "MCSMTPSendWithDataOperation.h"
#include "MCSMTPSendWithBuilderOperation.h"

using namespace mailcore;

SMTPAsyncSession::SMTPAsyncSession()
{
    mSession = new SMTPSession();
    mQueue = new OperationQueue();
}

SMTPAsyncSession::~SMTPAsyncSession()
{
    mQueue->release();
    mSession->release();
}

String * SMTPAsyncSession::className()
{
    return MCSTR("SMTPAsyncSession");
}

void SMTPAsyncSession::setHostname(String * hostname)
{
    mSession->setHostname(hostname);
}

String * SMTPAsyncSession::hostname()
{
    return mSession->hostname();
}

void SMTPAsyncSession::setPort(unsigned int port)
{
    mSession->setPort(port);
}

unsigned int SMTPAsyncSession::port()
{
    return mSession->port();
}

void SMTPAsyncSession::setUsername(String * username)
{
    return mSession->setUsername(username);
}

String * SMTPAsyncSession::username()
{
    return mSession->username();
}

void SMTPAsyncSession::setPassword(String * password)
{
    mSession->setPassword(password);
}

String * SMTPAsyncSession::password()
{
    return mSession->password();
}

void SMTPAsyncSession::setAuthType(AuthType authType)
{
    mSession->setAuthType(authType);
}

AuthType SMTPAsyncSession::authType()
{
    return mSession->authType();
}

void SMTPAsyncSession::setConnectionType(ConnectionType connectionType)
{
    mSession->setConnectionType(connectionType);
}

ConnectionType SMTPAsyncSession::connectionType()
{
    return mSession->connectionType();
}

void SMTPAsyncSession::setTimeout(time_t timeout)
{
    return mSession->setTimeout(timeout);
}

time_t SMTPAsyncSession::timeout()
{
    return mSession->timeout();
}

void SMTPAsyncSession::setCheckCertificateEnabled(bool enabled)
{
    return mSession->setCheckCertificateEnabled(enabled);
}

bool SMTPAsyncSession::isCheckCertificateEnabled()
{
    return mSession->isCheckCertificateEnabled();
}

void SMTPAsyncSession::setUseHeloIPEnabled(bool enabled)
{
    mSession->setUseHeloIPEnabled(enabled);
}

bool SMTPAsyncSession::useHeloIPEnabled()
{
    return mSession->useHeloIPEnabled();
}

SMTPOperation * sendMessage(Address * from, Array * recipients, Data * messageData)
{
#if 0
    SMTPSendWithRecipientOperation * op = new SMTPSendWithRecipientOperation();
    op->setFrom(from);
    op->setRecipients(recipients);
    op->messageData(messageData);
    return (SMTPOperation *) op->autorelease();
#endif
    return NULL;
}

SMTPOperation * sendMessage(Data * messageData)
{
#if 0
    SMTPSendWithDataOperation * op = new SMTPSendWithDataOperation();
    op->setData(messageData);
    return (SMTPOperation *) op->autorelease();
#endif
    return NULL;
}

SMTPOperation * sendMessage(MessageBuilder * msg)
{
#if 0
    SMTPSendWithBuilderOperation * op = new SMTPSendWithBuilderOperation();
    op->setBuilder(msg);
    return (SMTPOperation *) op->autorelease();
#endif
    return NULL;
}
