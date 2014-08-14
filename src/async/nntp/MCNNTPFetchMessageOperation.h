//
//  MCNNTPFetchMessagesOperation.h
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCNNTPFETCHMESSAGEOPERATION_H

#define MAILCORE_MCNNTPFETCHMESSAGEOPERATION_H

#include <MailCore/MCNNTPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    class NNTPFetchMessageOperation : public NNTPOperation {
    public:
        NNTPFetchMessageOperation();
        virtual ~NNTPFetchMessageOperation();
        
        virtual void setGroupName(String * groupName);
        virtual String * groupName();
        
        virtual void setMessageIndex(unsigned int messageIndex);
        virtual unsigned int messageIndex();
        
        virtual Data * data();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        String * mGroupName;
        unsigned int mMessageIndex;
        Data * mData;
        
    };
    
}

#endif

#endif
