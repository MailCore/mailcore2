//
//  IMAPSearchOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__IMAPSearchOperation__
#define __mailcore2__IMAPSearchOperation__

#include <mailcore/MCIMAPOperation.h>
#include <mailcore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPSearchExpression;
    
    class IMAPSearchOperation : public IMAPOperation {
    public:
        IMAPSearchOperation();
        virtual ~IMAPSearchOperation();
        
        virtual void setSearchKind(IMAPSearchKind kind);
        virtual IMAPSearchKind searchKind();
        
        virtual void setSearchString(String * searchString);
        virtual String * searchString();
        
        virtual void setSearchExpression(IMAPSearchExpression * expression);
        virtual IMAPSearchExpression * searchExpression();
        
        // Result.
        virtual Array * uids();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        IMAPSearchKind mKind;
        String * mSearchString;
        IMAPSearchExpression * mExpression;
        Array * mUids;
        
    };
}

#endif

#endif /* defined(__mailcore2__IMAPSearchOperation__) */
