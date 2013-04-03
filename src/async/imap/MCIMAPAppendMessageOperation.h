//
//  MCIMAPAppendMessageOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPAppendMessageOperation__
#define __mailcore2__MCIMAPAppendMessageOperation__

#include <MailCore/MCIMAPOperation.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPAppendMessageOperation : public IMAPOperation {
    public:
        IMAPAppendMessageOperation();
        virtual ~IMAPAppendMessageOperation();
        
        virtual void setMessageData(Data * messageData);
        virtual Data * messageData();
        
        virtual void setFlags(MessageFlag flags);
        virtual MessageFlag flags();
        
        virtual uint32_t createdUID();
        
    public: // subclass behavior
        virtual void main();

    private:
        Data * mMessageData;
        MessageFlag mFlags;
        uint32_t mCreatedUID;
        
    };
}

#endif

#endif /* defined(__mailcore2__MCIMAPAppendMessageOperation__) */
