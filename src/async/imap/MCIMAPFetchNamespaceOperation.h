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

namespace mailcore {
    
    class IMAPFetchNamespaceOperation : public IMAPOperation {
        HashMap * mNamespaces;
    private:
        
    public:
        IMAPFetchNamespaceOperation();
        virtual ~IMAPFetchNamespaceOperation();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__IMAPFetchNamespaceOperation__) */
