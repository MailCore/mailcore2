//
//  MCIMAPFolderStatusOperation.h
//  mailcore2
//
//  Created by Sebastian on 6/5/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//


#ifndef __MAILCORE_MCIMAPFOLDERSTATUSOPERATION_H_

#define __MAILCORE_MCIMAPFOLDERSTATUSOPERATION_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPFolderStatusOperation : public IMAPOperation {
    public:
        IMAPFolderStatusOperation();
        virtual ~IMAPFolderStatusOperation();
        
        virtual uint32_t uidNext();
        virtual uint32_t uidValidity();
        virtual uint32_t messageCount();
        virtual uint32_t recentCount();
        virtual uint32_t unseenCount();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        
         uint32_t mUidNext;
         uint32_t mUidValidity;
         uint32_t mMessageCount;
         uint32_t mRecentCount;
         uint32_t mUnseenCount;
    };
    
}

#endif

#endif
