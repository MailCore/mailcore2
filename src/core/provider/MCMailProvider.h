//
//  MCMailProvider.h
//  mailcore2
//
//  Created by Robert Widmann on 4/28/13.
//  Copyright (c) 2013 MailCore. All rights reserved.
//

#ifndef __MAILCORE_MCMAILPROVIDER_H_
#define __MAILCORE_MCMAILPROVIDER_H_

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {
    class NetService;
	
    class MailProvider : public Object {
        
    public:
        static MailProvider * providerWithInfo(HashMap * info);
        
		MailProvider();
		virtual ~MailProvider();
		
		virtual String * identifier();
		
		virtual Array * /* NetService */ imapServices();
		virtual Array * /* NetService */ smtpServices();
		virtual Array * /* NetService */ popServices();
		
		virtual bool matchEmail(String * email);
		virtual bool matchMX(String * hostname);
		
		virtual String * sentMailFolderPath();
		virtual String * starredFolderPath();
		virtual String * allMailFolderPath();
		virtual String * trashFolderPath();
		virtual String * draftsFolderPath();
		virtual String * spamFolderPath();
		virtual String * importantFolderPath();
		
        // Returns true if one of the folders above matches the given one.
		virtual bool isMainFolder(String * folderPath, String * prefix);
		
    public: // subclass behavior
		MailProvider(MailProvider * other);
		virtual String * description();
		virtual Object * copy();

    public: // private
		virtual void setIdentifier(String * identifier);
		virtual void fillWithInfo(HashMap * info);
        
	private:
		String * mIdentifier;
		Array * /* String */ mDomainMatch;
		Array * /* NetService */ mImapServices;
		Array * /* NetService */ mSmtpServices;
		Array * /* NetService */ mPopServices;
		HashMap * mMailboxPaths;
		Set * mMxSet;
		
		void init();
    };
};

#endif

#endif