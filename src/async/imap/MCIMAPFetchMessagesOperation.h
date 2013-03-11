//
//  IMAPFetchMessagesOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__IMAPFetchMessagesOperation__
#define __mailcore2__IMAPFetchMessagesOperation__

#include <mailcore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPFetchMessagesOperation : public IMAPOperation {
    public:
        IMAPFetchMessagesOperation();
        virtual ~IMAPFetchMessagesOperation();
        
        virtual void setFetchByUidEnabled(bool enabled);
        virtual bool isFetchByUidEnabled();
        
#if 0
        virtual void setFirst(uint32_t first);
        virtual uint32_t first();
        
        virtual void setLast(uint32_t last);
        virtual uint32_t last();
        
        virtual void setUids(Array * uids);
        virtual Array * uids();
        
        virtual void setNumbers(Array * numbers);
        virtual Array * numbers();
#endif
        virtual void setIndexes(IndexSet * indexes);
        virtual IndexSet * indexes();
        
        virtual void setModSequenceValue(uint64_t modseq);
        virtual uint64_t modSequenceValue();
        
        virtual void setKind(IMAPMessagesRequestKind kind);
        virtual IMAPMessagesRequestKind kind();
        
        // Result.
        virtual Array * /* IMAPMessage */ messages();
        virtual Array * /* Value */ vanishedMessages();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        bool mFetchByUidEnabled;
#if 0
        uint32_t mFirst;
        uint32_t mLast;
        Array * mUids;
        Array * mNumbers;
#endif
        IndexSet * mIndexes;
        IMAPMessagesRequestKind mKind;
        Array * mMessages;
        Array * mVanishedMessages;
        uint64_t mModSequenceValue;
        
    };
}

#endif

#endif /* defined(__mailcore2__IMAPFetchMessagesOperation__) */
