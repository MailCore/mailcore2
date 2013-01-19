//
//  main.m
//  tests
//
//  Created by DINH Viêt Hoà on 1/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#import <Foundation/Foundation.h>

#include <unicode/putil.h>
#include <unicode/uclean.h>
#include <unicode/ucnv.h>
#include <mailcore/mailcore.h>

extern "C" {
    extern int mailstream_debug;
}

static mailcore::String * password = NULL;
static mailcore::String * displayName = NULL;
static mailcore::String * email = NULL;

class TestOperation : public mailcore::Operation {
	void main()
	{
		MCLog("coin %p", this);
	}
};

class TestCallback : public mailcore::Object, public mailcore::OperationCallback {
	virtual void operationFinished(mailcore::Operation * op)
	{
		MCLog("callback coin %p %p %s", this, op, MCUTF8DESC(this));
	}
};

static mailcore::Data * testMessageBuilder()
{
    mailcore::Address * address = new mailcore::Address();
    address->setDisplayName(displayName);
    address->setMailbox(email);
    
    address->release();
    
    mailcore::MessageBuilder * msg = new mailcore::MessageBuilder();
    
    msg->header()->setFrom(mailcore::Address::addressWithDisplayName(displayName, email));
    mailcore::Array * to = new mailcore::Array();
    mailcore::Array * bcc = new mailcore::Array();
    to->addObject(mailcore::Address::addressWithDisplayName(MCSTR("Foo Bar"), MCSTR("foobar@to-recipient.org")));
    to->addObject(mailcore::Address::addressWithDisplayName(MCSTR("Other Recipient"), MCSTR("another-foobar@to-recipient.org")));
    bcc->addObject(mailcore::Address::addressWithDisplayName(MCSTR("Hidden Recipient"), MCSTR("foobar@bcc-recipient.org")));
    msg->header()->setTo(to);
    msg->header()->setBcc(bcc);
    to->release();
    bcc->release();
    msg->header()->setSubject(MCSTR("Mon projet d'été"));
    msg->setHTMLBody(MCSTR("<div>Hello <img src=\"cid:1234\"></div>"));
    msg->addAttachment(mailcore::Attachment::attachmentWithContentOfFile(MCSTR("first-filename")));
    msg->addAttachment(mailcore::Attachment::attachmentWithContentOfFile(MCSTR("second-filename")));
    mailcore::Attachment * attachment = mailcore::Attachment::attachmentWithContentOfFile(MCSTR("third-image-attachment"));
    attachment->setContentID(MCSTR("1234"));
    msg->addRelatedAttachment(attachment);
    
    mailcore::Data * data = msg->data();
    
    MCLog("%s", data->bytes());
    
    msg->release();
    
    return data;
}

static void testMessageParser(mailcore::Data * data)
{
    mailcore::MessageParser * parser = mailcore::MessageParser::messageParserWithData(data);
    MCLog("%s", MCUTF8(parser->description()));
}

static void testIMAP()
{
    mailcore::IMAPSession * session;
    mailcore::ErrorCode error;
    
    session = new mailcore::IMAPSession();
    session->setHostname(MCSTR("imap.gmail.com"));
    session->setPort(993);
    session->setUsername(email);
    session->setPassword(password);
    session->setConnectionType(mailcore::ConnectionTypeTLS);
    
    mailcore::IMAPMessagesRequestKind requestKind = (mailcore::IMAPMessagesRequestKind)
    (mailcore::IMAPMessagesRequestKindHeaders | mailcore::IMAPMessagesRequestKindStructure |
     mailcore::IMAPMessagesRequestKindInternalDate | mailcore::IMAPMessagesRequestKindHeaderSubject |
     mailcore::IMAPMessagesRequestKindFlags);
    mailcore::Array * messages = session->fetchMessagesByUID(MCSTR("INBOX"),
                                                              requestKind, 1, 0, NULL, &error);
    MCLog("%s", MCUTF8DESC(messages));
    
    session->release();
}

static void testSMTP(mailcore::Data * data)
{
    mailcore::SMTPSession * smtp;
    mailcore::ErrorCode error;
    
    smtp = new mailcore::SMTPSession();
    
    smtp->setHostname(MCSTR("smtp.gmail.com"));
    smtp->setPort(25);
    smtp->setUsername(email);
    smtp->setPassword(password);
    smtp->setConnectionType(mailcore::ConnectionTypeStartTLS);
    
    smtp->sendMessage(data, NULL, &error);
    
    smtp->release();
}

static void testPOP()
{
    mailcore::POPSession * session;
    mailcore::ErrorCode error;
    
    session = new mailcore::POPSession();
    session->setHostname(MCSTR("pop.gmail.com"));
    session->setPort(995);
    session->setUsername(email);
    session->setPassword(password);
    session->setConnectionType(mailcore::ConnectionTypeTLS);
    
    mailcore::Array * messages = session->fetchMessages(&error);
    MCLog("%s", MCUTF8DESC(messages));
    
    session->release();
}

static void testOperationQueue()
{
    mailcore::OperationQueue * queue = new mailcore::OperationQueue();
    
	TestCallback * callback = new TestCallback();
	
    for(unsigned int i = 0 ; i < 100 ; i ++) {
        mailcore::Operation * op = new TestOperation();
		op->setCallback(callback);
        queue->addOperation(op);
        op->release();
    }
    
    [[NSRunLoop currentRunLoop] run];
    
    queue->release();
}

class TestSMTPCallback : public mailcore::Object, public mailcore::OperationCallback, public mailcore::SMTPOperationCallback {
	virtual void operationFinished(mailcore::Operation * op)
	{
		MCLog("callback %s %s", MCUTF8DESC(op), MCUTF8DESC(this));
	}
    
    virtual void bodyProgress(mailcore::SMTPOperation * op, unsigned int current, unsigned int maximum)
    {
		MCLog("progress %s %s %i/%i", MCUTF8DESC(op), MCUTF8DESC(this), current, maximum);
    }
};

static void testAsyncSMTP(mailcore::Data * data)
{
    mailcore::SMTPAsyncSession * smtp;
    TestSMTPCallback * callback = new TestSMTPCallback();
    
    smtp = new mailcore::SMTPAsyncSession();
    
    smtp->setHostname(MCSTR("smtp.gmail.com"));
    smtp->setPort(25);
    smtp->setUsername(email);
    smtp->setPassword(password);
    smtp->setConnectionType(mailcore::ConnectionTypeStartTLS);
    
    mailcore::SMTPOperation * op = smtp->sendMessageOperation(data);
    op->setSmtpCallback(callback);
    op->setCallback(callback);
    op->start();
    
    [[NSRunLoop currentRunLoop] run];
    
    //smtp->release();
}

class TestIMAPCallback : public mailcore::Object, public mailcore::OperationCallback, public mailcore::IMAPOperationCallback {
	virtual void operationFinished(mailcore::Operation * op)
	{
        mailcore::IMAPFetchMessagesOperation * fetchOp = (mailcore::IMAPFetchMessagesOperation *) op;
		//MCLog("callback %s %s %s", MCUTF8DESC(op), MCUTF8DESC(fetchOp->messages()), MCUTF8DESC(this));
	}
    
    virtual void bodyProgress(mailcore::IMAPOperation * op, unsigned int current, unsigned int maximum)
    {
		MCLog("progress %s %s %i/%i", MCUTF8DESC(op), MCUTF8DESC(this), current, maximum);
    }
    
    virtual void itemProgress(mailcore::IMAPOperation * op, unsigned int current, unsigned int maximum)
    {
		MCLog("item progress %s %s %i/%i", MCUTF8DESC(op), MCUTF8DESC(this), current, maximum);
    }
};

static void testAsyncIMAP()
{
    mailcore::IMAPAccount * session;
    TestIMAPCallback * callback = new TestIMAPCallback();
    
    session = new mailcore::IMAPAccount();
    session->setHostname(MCSTR("imap.gmail.com"));
    session->setPort(993);
    session->setUsername(email);
    session->setPassword(password);
    session->setConnectionType(mailcore::ConnectionTypeTLS);
    
    mailcore::IMAPMessagesRequestKind requestKind = (mailcore::IMAPMessagesRequestKind)
    (mailcore::IMAPMessagesRequestKindHeaders | mailcore::IMAPMessagesRequestKindStructure |
     mailcore::IMAPMessagesRequestKindInternalDate | mailcore::IMAPMessagesRequestKindHeaderSubject |
     mailcore::IMAPMessagesRequestKindFlags);
    mailcore::IMAPFetchMessagesOperation * op = session->fetchMessagesByUIDOperation(MCSTR("INBOX"), requestKind, 1, 0);
    op->setCallback(callback);
    op->setImapCallback(callback);
    op->start();
    //MCLog("%s", MCUTF8DESC(messages));
    [[NSRunLoop currentRunLoop] run];
    
    //session->release();
}

class TestPOPCallback : public mailcore::Object, public mailcore::OperationCallback, public mailcore::POPOperationCallback {
	virtual void operationFinished(mailcore::Operation * op)
	{
        mailcore::POPFetchMessagesOperation * fetchOp = (mailcore::POPFetchMessagesOperation *) op;
		MCLog("callback %s %s", MCUTF8DESC(fetchOp->messages()), MCUTF8DESC(this));
	}
    
    virtual void bodyProgress(mailcore::IMAPOperation * op, unsigned int current, unsigned int maximum)
    {
		MCLog("progress %s %s %i/%i", MCUTF8DESC(op), MCUTF8DESC(this), current, maximum);
    }
};

static void testAsyncPOP()
{
    mailcore::POPAsyncSession * session;
    TestPOPCallback * callback = new TestPOPCallback();
    
    session = new mailcore::POPAsyncSession();
    session->setHostname(MCSTR("pop.gmail.com"));
    session->setPort(995);
    session->setUsername(email);
    session->setPassword(password);
    session->setConnectionType(mailcore::ConnectionTypeTLS);
    
    mailcore::POPFetchMessagesOperation * op = session->fetchMessagesOperation();
    op->setCallback(callback);
    op->setPopCallback(callback);
    op->start();
    
    //mailcore::Array * messages = session->fetchMessages(&error);
    //MCLog("%s", MCUTF8DESC(messages));
    
    [[NSRunLoop currentRunLoop] run];
}

void testAll()
{
    u_setDataDirectory("/usr/local/share/icu");
    
    mailcore::AutoreleasePool * pool = new mailcore::AutoreleasePool();
    
    mailstream_debug = 1;
    
    //mailcore::Data * data = testMessageBuilder();
    //testMessageParser(data);
    //testSMTP(data);
    //testIMAP();
    //testPOP();
    //testAsyncSMTP(data);
    //testAsyncIMAP();
    //testAsyncPOP();
    
    MCLog("pool release");
    pool->release();
}

int main(int argc, const char * argv[])
{

    @autoreleasepool {
        
        email = MCSTR("email@gmail.com");
        password = MCSTR("MyP4ssw0rd");
        displayName = MCSTR("My Email");
        
        testAll();
        
    }
    return 0;
}

