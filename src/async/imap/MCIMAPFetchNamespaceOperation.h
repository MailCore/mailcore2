//
//  IMAPFetchNamespaceOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__IMAPFetchNamespaceOperation__
#define __mailcore2__IMAPFetchNamespaceOperation__

#include <mailcore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPFetchNamespaceOperation : public IMAPOperation {
    public:
        IMAPFetchNamespaceOperation();
        virtual ~IMAPFetchNamespaceOperation();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        HashMap * mNamespaces;
        
    };
}

#endif

#endif /* defined(__mailcore2__IMAPFetchNamespaceOperation__) */
