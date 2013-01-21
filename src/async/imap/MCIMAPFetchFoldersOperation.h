//
//  MCIMAPFetchFoldersOperation.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 1/12/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPFetchFoldersOperation__
#define __mailcore2__MCIMAPFetchFoldersOperation__

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCIMAPOperation.h>

#ifdef __cplusplus

namespace mailcore {
    
    class IMAPFetchFoldersOperation : public IMAPOperation {
    public:
        IMAPFetchFoldersOperation();
        virtual ~IMAPFetchFoldersOperation();
        
        virtual void setFetchSubscribedEnabled(bool enabled);
        virtual bool isFetchSubscribedEnabled();
        
        virtual Array * folders();
        
    public: // subclass behavior
        virtual void main();
        
    private:
        String * mFolder;
        bool mFetchSubscribedEnabled;
        Array * mFolders;
        void setDelimiterDataOnMainThread(char * delimiterData);
        
    };
}

#endif

#endif /* defined(__mailcore2__MCIMAPFetchFoldersOperation__) */
