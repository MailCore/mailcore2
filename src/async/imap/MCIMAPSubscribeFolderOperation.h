//
//  MCIMAPSubscribeFolderOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPSUBSCRIBEFOLDEROPERATION_H_

#define __MAILCORE_MCIMAPSUBSCRIBEFOLDEROPERATION_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPSubscribeFolderOperation : public IMAPOperation {
    public:
        IMAPSubscribeFolderOperation();
        virtual ~IMAPSubscribeFolderOperation();
        
        virtual void setUnsubscribeEnabled(bool enabled);
        virtual bool isUnsubscribeEnabled();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        bool mUnsubscribeEnabled;
        
    };
}

#endif

#endif
