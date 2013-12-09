//
//  MCIMAPNoopOperation.h
//  mailcore2
//
//  Created by Robert Widmann on 9/24/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPNOOPOPERATION_H_

#define __MAILCORE_MCIMAPNOOPOPERATION_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPNoopOperation : public IMAPOperation {
    public:
        IMAPNoopOperation();
        virtual ~IMAPNoopOperation();
        
    public: // subclass behavior
        virtual void main();
    };
    
}

#endif

#endif
