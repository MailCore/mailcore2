//
//  MCIMAPCapabilityOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/4/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPCapabilityOperation__
#define __mailcore2__MCIMAPCapabilityOperation__

#include <mailcore/MCIMAPOperation.h>

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

#endif /* defined(__mailcore2__MCIMAPCapabilityOperation__) */
