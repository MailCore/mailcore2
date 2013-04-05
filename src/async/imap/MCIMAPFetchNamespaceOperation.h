//
//  IMAPFetchNamespaceOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPFETCHNAMESPACEOPERATION_H_

#define __MAILCORE_MCIMAPFETCHNAMESPACEOPERATION_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPFetchNamespaceOperation : public IMAPOperation {
    public:
        IMAPFetchNamespaceOperation();
        virtual ~IMAPFetchNamespaceOperation();
        
        // Result.
        virtual HashMap * namespaces();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        HashMap * mNamespaces;
        
    };
}

#endif

#endif
