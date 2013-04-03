//
//  MCIMAPSyncResult.h
//  mailcore2
//
//  Created by DINH Viêt Hoà on 3/3/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCIMAPSyncResult__
#define __mailcore2__MCIMAPSyncResult__

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {
 
    class IMAPSyncResult : public Object {
	public:
		IMAPSyncResult();
		virtual ~IMAPSyncResult();
		
        virtual void setModifiedOrAddedMessages(Array * /* IMAPMessage */ messages);
        virtual Array * /* IMAPMessage */ modifiedOrAddedMessages();
        
        virtual void setVanishedMessages(IndexSet * vanishedMessages);
        virtual IndexSet * vanishedMessages();
        
    private:
        Array * /* IMAPMessage */ mModifiedOrAddedMessages;
        IndexSet * mVanishedMessages;
    };
    
}

#endif

#endif /* defined(__mailcore2__MCIMAPSyncResult__) */
