//
//  MCIMAPSubscribeFolderOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPSubscribeFolderOperation__
#define __mailcore2__MCIMAPSubscribeFolderOperation__

#include <mailcore/MCIMAPOperation.h>

namespace mailcore {
    
    class IMAPSubscribeFolderOperation : public IMAPOperation {
    private:
        bool mUnsubscribeEnabled;
        
    public:
        IMAPSubscribeFolderOperation();
        virtual ~IMAPSubscribeFolderOperation();
        
        virtual void setUnsubscribeEnabled(bool enabled);
        virtual bool isUnsubscribeEnabled();
        
        virtual void main();
    };
}

#endif /* defined(__mailcore2__MCIMAPSubscribeFolderOperation__) */
