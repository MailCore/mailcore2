//
//  MCIMAPStoreLabelsOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPSTORELABELSOPERATION_H_

#define __MAILCORE_MCIMAPSTORELABELSOPERATION_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPStoreLabelsOperation : public IMAPOperation {
    public:
        IMAPStoreLabelsOperation();
        virtual ~IMAPStoreLabelsOperation();
        
        virtual void setUids(IndexSet * uids);
        virtual IndexSet * uids();
        
        virtual void setKind(IMAPStoreFlagsRequestKind kind);
        virtual IMAPStoreFlagsRequestKind kind();
        
        virtual void setLabels(Array * /* String */ labels);
        virtual Array * /* String */ labels();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        IndexSet * mUids;
        IMAPStoreFlagsRequestKind mKind;
        Array * /* String */ mLabels;
        
    };
}

#endif

#endif
