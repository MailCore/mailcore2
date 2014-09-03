//
//  MCIMAPFolderInfoOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/13/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCIMAPFOLDERINFOOPERATION_H

#define MAILCORE_MCIMAPFOLDERINFOOPERATION_H

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPFolderInfoOperation : public IMAPOperation {
    public:
        IMAPFolderInfoOperation();
        virtual ~IMAPFolderInfoOperation();
        
        virtual uint32_t uidNext();
        virtual uint32_t uidValidity();
        virtual uint64_t modSequenceValue();
        virtual int messageCount();
        virtual uint32_t firstUnseenUid();
        virtual bool allowsNewPermanentFlags();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        
        uint32_t mUidNext;
        uint32_t mUidValidity;
        uint64_t mModSequenceValue;
        int mMessageCount;
        uint32_t mFirstUnseenUid;
        bool mAllowsNewPermanentFlags;
      
    };

}

#endif

#endif
