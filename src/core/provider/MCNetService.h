//
//  MCNetService.h
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE__MCNetService__
#define __MAILCORE__MCNetService__

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {
	class NetService : public Object {
		
	public:
		NetService(HashMap * info);
		~NetService();

		void setHostname(String * hostname);
		String * hostname();
		
		void setPort(unsigned int port);
		unsigned int port();
		
		void setConnectionType(ConnectionType connectionType);
		ConnectionType connectionType();
				
		HashMap * info();
		
		String * normalizedHostnameWithEmail(String * email);
				
	private:
		String * mHostname;
		unsigned int mPort;
		ConnectionType mConnectionType;
		
		void init();
	};
}

#endif
	
#endif /* defined(__MAILCORE__MCNetService__) */
