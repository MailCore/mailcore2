//
//  MCOperationQueueCallback.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 6/22/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_OPERATIONQUEUECALLBACK_H_
#define __MAILCORE_OPERATIONQUEUECALLBACK_H_

namespace mailcore {
	
	class OperationQueue;
    
	class OperationQueueCallback {
    public:
        virtual void queueIdle() {}
    };
}

#endif
