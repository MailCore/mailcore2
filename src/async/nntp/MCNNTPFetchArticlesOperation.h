//
//  MCMCNNTPFetchArticlesOperation.h
//  mailcore2
//
//  Created by Robert Widmann on 8/13/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#ifndef MAILCORE_MCNNTPFETCHMESSAGESOPERATION_H

#define MAILCORE_MCNNTPFETCHMESSAGESOPERATION_H

#include <MailCore/MCNNTPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class MCNNTPFetchArticlesOperation : public NNTPOperation {
    public:
        MCNNTPFetchArticlesOperation();
        virtual ~MCNNTPFetchArticlesOperation();
        
        virtual void setGroupName(String * groupName);
        virtual String * groupName();
        
        virtual Array * /* NNTPArticleInfo */ messages();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        String * mGroupName;
        Array * /* NNTPArticleInfo */ mMessages;
    };
    
}

#endif

#endif
