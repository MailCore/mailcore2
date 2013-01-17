//
//  MCPOPOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/16/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCPOPOperation__
#define __mailcore2__MCPOPOperation__

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCPOPProgressCallback.h>

namespace mailcore {
    
    class POPAsyncSession;
    class POPOperationCallback;
    
    class POPOperation : public Operation, public POPProgressCallback {
        POPAsyncSession * mSession;
        POPOperationCallback * mPopCallback;
        ErrorCode mError;
    private:
		virtual void bodyProgress(POPSession * session, unsigned int current, unsigned int maximum);
        virtual void bodyProgressOnMainThread(void * context);
        
    public:
        POPOperation();
        virtual ~POPOperation();
        
        virtual void setSession(POPAsyncSession * session);
        virtual POPAsyncSession * session();
        
        virtual void setPopCallback(POPOperationCallback * callback);
        virtual POPOperationCallback * popCallback();
        
        virtual void setError(ErrorCode error);
        virtual ErrorCode error();
        
        virtual void start();
    };
    
}

#endif /* defined(__mailcore2__MCPOPOperation__) */
