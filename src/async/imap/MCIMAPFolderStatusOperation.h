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
    
    class IMAPFolderStatus;
    
    class IMAPFolderStatusOperation : public IMAPOperation {
    public:
        IMAPFolderStatusOperation();
        virtual ~IMAPFolderStatusOperation();
        
        // Results.
        virtual IMAPFolderStatus * status();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        IMAPFolderStatus * mStatus;
    };
    
}

#endif

#endif
