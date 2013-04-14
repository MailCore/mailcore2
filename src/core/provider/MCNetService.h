//
//  MCONetService.h
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCONetService__
#define __mailcore2__MCONetService__

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {
	
	class NetService : public Object {
		
	public:
		NetService();
		NetService(HashMap * info);
		virtual ~NetService();
		
		virtual void setHostname(String * hostname);
		virtual String * hostname();
		
		virtual void setPort(unsigned int port);
		virtual unsigned int port();
		
		virtual void setAuthType(AuthType authType);
		virtual AuthType authType();
		
		virtual HashMap * info();
				
	private:
		String * mHostname;
		unsigned int mPort;
		AuthType mAuthType;
		
		void init();
	};
	
}

#endif

#endif /* defined(__mailcore2__MCONetService__) */
