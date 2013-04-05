//
//  MCIMAPCapabilityOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/4/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPCAPABILITYOPERATION_H_

#define __MAILCORE_MCIMAPCAPABILITYOPERATION_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPCapabilityOperation : public IMAPOperation {
    public:
        IMAPCapabilityOperation();
        virtual ~IMAPCapabilityOperation();
        
        // Result.
        virtual IndexSet * capabilities();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        IndexSet * mCapabilities;
    };
}

#endif

#endif
