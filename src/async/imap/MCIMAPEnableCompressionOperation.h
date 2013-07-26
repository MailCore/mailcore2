//
//  MCIMAPEnableCompressionOperation.h
//  mailcore2
//
//  Created by Matt Ronge on 7/17/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPEnableCompressionOperation__
#define __mailcore2__MCIMAPEnableCompressionOperation__

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPEnableCompressionOperation : public IMAPOperation {
    public: // subclass behavior
        virtual void main();
    };
}

#endif

#endif /* defined(__mailcore2__MCIMAPEnableCompressionOperation__) */
