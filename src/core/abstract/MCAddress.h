#ifndef __MAILCORE_MCADDRESS_H_

#define __MAILCORE_MCADDRESS_H_

#include <libetpan/libetpan.h>
#include <mailcore/MCBaseTypes.h>

namespace mailcore {

	class Address : public Object {
	private:
		String * mDisplayName;
		String * mMailbox;
		void init();

	public:
		Address();
		Address(Address * other);
		virtual ~Address();

		static Address * addressWithDisplayName(String * displayName, String * mailbox);
		static Address * addressWithMailbox(String * mailbox);
		static Address * addressWithRFC822String(String * RFC822String);
		static Address * addressWithNonEncodedRFC822String(String * nonEncodedRFC822String);

		virtual String * description();

		virtual bool isEqual(Object * otherObject);
		virtual unsigned int hash();

		virtual Object * copy();
		
		virtual void setDisplayName(String * displayName);
		virtual String * displayName();

		virtual void setMailbox(String * address);
		virtual String * mailbox();

		virtual String * RFC822String();
		virtual String * nonEncodedRFC822String();
		
		// Additions
		static Address * addressWithIMFMailbox(struct mailimf_mailbox * mb);
		static Address * addressWithNonEncodedIMFMailbox(struct mailimf_mailbox * mb);
		static Address * addressWithIMAPAddress(struct mailimap_address * imap_addr);
		
		// Must be released
		virtual struct mailimf_address * createIMFAddress();
		virtual struct mailimf_mailbox * createIMFMailbox();
	};

}

#endif
