//
//  MCMailProvider.h
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE__MCMailProvider__
#define __MAILCORE__MCMailProvider__

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {
    class NetService;
	
    class MailProvider : public Object {
        
    public:
		MailProvider(HashMap * info);
		MailProvider(Data * infoData);
		~MailProvider();
		
		void setIdentifier(String * identifier);
		String * identifier();
		
		Array * imapServices();
		Array * smtpServices();
		Array * popServices();
		
		bool matchEmail(String * email);
		bool matchMX(String * hostname);
		
		String * sentMailFolderPath();
		String * starredFolderPath();
		String * allMailFolderPath();
		String * trashFolderPath();
		String * draftsFolderPath();
		String * spamFolderPath();
		String * importantFolderPath();
		
		bool isMainFolder(String * folderPath, String * prefix);
		
		virtual String * description();
		
	private:
		String * mIdentifier;
		Array * mDomainMatch;
		Array * mImapServices;
		Array * mSmtpServices;
		Array * mPopServices;
		HashMap * mMailboxPaths;
		Set* mMxSet;
		
		void init();
		void initWihInfo(HashMap * info);
    };
};

#endif

#endif /* defined(__MAILCORE__MCMailProvider__) */
