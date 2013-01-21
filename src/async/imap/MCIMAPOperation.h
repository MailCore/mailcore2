//
//  MCIMAPOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPOperation__
#define __mailcore2__MCIMAPOperation__

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCIMAPProgressCallback.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPAsyncConnection;
    class IMAPOperationCallback;
    
    class IMAPOperation : public Operation, public IMAPProgressCallback {
    public:
        IMAPOperation();
        virtual ~IMAPOperation();
        
        virtual void setSession(IMAPAsyncConnection * session);
        virtual IMAPAsyncConnection * session();
        
        virtual void setFolder(String * folder);
        virtual String * folder();
        
        virtual void setImapCallback(IMAPOperationCallback * callback);
        virtual IMAPOperationCallback * imapCallback();
        
        virtual void setError(ErrorCode error);
        virtual ErrorCode error();
        
        virtual void start();
        
    private:
        IMAPAsyncConnection * mSession;
        String * mFolder;
        IMAPOperationCallback * mImapCallback;
        ErrorCode mError;
        
    private:
		virtual void bodyProgress(IMAPSession * session, unsigned int current, unsigned int maximum);
        virtual void bodyProgressOnMainThread(void * context);
        virtual void itemsProgress(IMAPSession * session, unsigned int current, unsigned int maximum);
        virtual void itemsProgressOnMainThread(void * context);
        
    };
    
}

#endif

#endif /* defined(__mailcore2__MCIMAPOperation__) */
