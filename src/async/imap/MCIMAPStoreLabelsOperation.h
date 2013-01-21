//
//  MCIMAPStoreLabelsOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPStoreLabelsOperation__
#define __mailcore2__MCIMAPStoreLabelsOperation__

#include <mailcore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPStoreLabelsOperation : public IMAPOperation {
    public:
        IMAPStoreLabelsOperation();
        virtual ~IMAPStoreLabelsOperation();
        
        virtual void setUids(Array * uids);
        virtual Array * uids();
        
        virtual void setKind(IMAPStoreFlagsRequestKind kind);
        virtual IMAPStoreFlagsRequestKind kind();
        
        virtual void setLabels(Array * labels);
        virtual Array * labels();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        Array * mUids;
        IMAPStoreFlagsRequestKind mKind;
        Array * mLabels;
        
    };
}

#endif

#endif /* defined(__mailcore2__MCIMAPStoreLabelsOperation__) */
