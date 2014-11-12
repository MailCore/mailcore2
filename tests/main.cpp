//
//  main.m
//  tests
//
//  Created by DINH Viêt Hoà on 1/10/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#include <MailCore/MailCore.h>
#include <glib.h>

using namespace mailcore;

class MyCallback : public OperationCallback {
  virtual void operationFinished(Operation * op) {
    fprintf(stderr, "operation finished\n");
    IMAPFetchFoldersOperation * folderOp = (IMAPFetchFoldersOperation *) op;
    fprintf(stderr, "result %s", MCUTF8(folderOp->folders()));
  }
};

int main(int argc, const char * argv[])
{

    GMainLoop * main_loop = g_main_loop_new (NULL, FALSE);

	IMAPAsyncSession * session = new IMAPAsyncSession();
    session->setHostname(MCSTR("imap.gmail.com"));
    session->setPort(993);
    session->setUsername(MCSTR("johndoe@gmail.com"));
    session->setPassword(MCSTR("blah"));
    session->setConnectionType(ConnectionTypeTLS);
    MyCallback * callback = new MyCallback();
    IMAPFetchFoldersOperation * op = session->fetchAllFoldersOperation();
    op->setCallback(callback);
    op->start();
    fprintf(stderr, "started op\n");

    g_main_loop_run(main_loop);

    return 0;
}

