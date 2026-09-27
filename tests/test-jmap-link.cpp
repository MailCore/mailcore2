#include <MailCore/MCAsyncJMAP.h>
#include <MailCore/MCJMAP.h>
#include <MailCore/MCMessageHeader.h>

int main()
{
    mailcore::JMAPSession * session = new mailcore::JMAPSession();
    session->setSessionURL(MCSTR("https://example.com/jmap/session"));
    session->setUsername(MCSTR("user@example.com"));
    session->setOAuth2Token(MCSTR("token"));
    session->setCheckCertificateEnabled(true);
    session->release();

    mailcore::JMAPAsyncSession * asyncSession = new mailcore::JMAPAsyncSession();
    asyncSession->setSessionURL(MCSTR("https://example.com/jmap/session"));
    asyncSession->setUsername(MCSTR("user@example.com"));
    asyncSession->setOAuth2Token(MCSTR("token"));
    asyncSession->setCheckCertificateEnabled(true);

    asyncSession->connectOperation();
    asyncSession->release();

    mailcore::JMAPMessage * message = new mailcore::JMAPMessage();
    message->setIdentifier(MCSTR("message-id"));
    message->header()->setSubject(MCSTR("subject"));
    message->release();

    return 0;
}
