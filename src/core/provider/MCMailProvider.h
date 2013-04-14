//
//  MCOMailProvider.h
//  mailcore2
//
//  Created by Robert Widmann on 4/14/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __mailcore2__MCOMailProvider__
#define __mailcore2__MCOMailProvider__

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {
	
	class MailProvider : public Object {
		
	public:
		MailProvider();
		MailProvider(HashMap * info);
		virtual ~MailProvider();
		
		virtual void setIdentifier(String * hostname);
		virtual String * identifier();
		
		virtual Array * imapServices();
		virtual Array * smtpServices();
		virtual Array * popServices();

		bool matchEmail(String * email);
		bool matchMX(String * hostname);

		virtual String * sentMailFolderPath();
		virtual String * starredFolderPath();
		virtual String * allMailFolderPath();
		virtual String * trashFolderPath();
		virtual String * draftsFolderPath();
		virtual String * spamFolderPath();
		virtual String * importantFolderPath();
		
	private:
		String * mIdentifier;
		Array * mDomainMatch;
		Array * mImapServices;
		Array * mSmtpServices;
		Array * mPopServices;
		HashMap * mMailboxPaths;
		Set * mMxSet;
		
		void init();
	};
	
}

#endif

#endif /* defined(__mailcore2__MCOMailProvider__) */
