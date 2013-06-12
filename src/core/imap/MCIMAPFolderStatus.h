//
//  MCIMAPFolderStatus.h
//  mailcore2
//
//  Created by Sebastian on 6/11/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCIMAPFOLDERSTATUS_H

#define __MAILCORE_MCIMAPFOLDERSTATUS_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {
	
	class IMAPFolderStatus : public Object {
	public:
        
        IMAPFolderStatus();
        virtual ~IMAPFolderStatus();
        		
		virtual void setUnreadCount(u_int32_t unread);
		virtual u_int32_t unreadCount();
		
		virtual void setMessageCount(u_int32_t messages);
		virtual u_int32_t messageCount();
        
        virtual void setRecentCount(u_int32_t recent);
		virtual u_int32_t recentCount();
		
        virtual void setUidNext(u_int32_t uidNext);
		virtual u_int32_t uidNext();                
        
        virtual void setUidValidity(u_int32_t uidValidity);
		virtual u_int32_t uidValidity();

        
    public: // subclass behavior
		IMAPFolderStatus(IMAPFolderStatus * other);
		virtual Object * copy();		
        
	private:
		u_int32_t mUnreadCount;
        u_int32_t mMessageCount;
        u_int32_t mRecentCount;
        u_int32_t mUidNext;
        u_int32_t mUidValidity;
        
		void init();
	};
	
}

#endif

#endif

