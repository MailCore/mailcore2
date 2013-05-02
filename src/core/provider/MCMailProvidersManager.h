//
//  MCMailProvidersManager.h
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE__MCMailProvidersManager__
#define __MAILCORE__MCMailProvidersManager__

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {
	class MailProvider;
	
	class MailProvidersManager : public Object {
	
	public:
		static MailProvidersManager * sharedManager()
		{
			static MailProvidersManager * instance = new MailProvidersManager();
			instance->init();
			return instance;
		}
		
		MailProvider * providerForEmail(String * email);
		MailProvider * providerForMX(String * hostname);
		MailProvider * providerForIdentifier(String * identifier);
		
		void registerProviders(HashMap * providers);
		void registerProvidersFilename(String * filename);
		
	private:
		MailProvidersManager();
		MailProvidersManager(MailProvidersManager const&);
		void operator=(MailProvidersManager const&);
		
		HashMap * mProviders;
		
		void init();
	};
}

#endif

#endif /* defined(__MAILCORE__MCMailProvidersManager__) */
