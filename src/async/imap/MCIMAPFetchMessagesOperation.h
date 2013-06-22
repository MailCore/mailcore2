//
//  IMAPFetchMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPFETCHMESSAGESOPERATION_H_

#define __MAILCORE_MCIMAPFETCHMESSAGESOPERATION_H_

#include <MailCore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPFetchMessagesOperation : public IMAPOperation {
    public:
        IMAPFetchMessagesOperation();
        virtual ~IMAPFetchMessagesOperation();
        
        virtual void setFetchByUidEnabled(bool enabled);
        virtual bool isFetchByUidEnabled();
        
        virtual void setIndexes(IndexSet * indexes);
        virtual IndexSet * indexes();
        
        virtual void setModSequenceValue(uint64_t modseq);
        virtual uint64_t modSequenceValue();
        
        virtual void setKind(IMAPMessagesRequestKind kind);
        virtual IMAPMessagesRequestKind kind();
        
        // Result.
        virtual Array * /* IMAPMessage */ messages();
        virtual IndexSet * vanishedMessages();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        bool mFetchByUidEnabled;
        IndexSet * mIndexes;
        IMAPMessagesRequestKind mKind;
        Array * /* IMAPMessage */ mMessages;
        IndexSet * mVanishedMessages;
        uint64_t mModSequenceValue;
        
    };
}

#endif

#endif
