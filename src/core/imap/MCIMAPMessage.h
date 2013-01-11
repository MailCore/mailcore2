#ifndef __MAILCORE_IMAP_MESSAGE_H_

#define __MAILCORE_IMAP_MESSAGE_H_

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractMessage.h>
#include <mailcore/MCMessageConstants.h>
#include <mailcore/MCAbstractPart.h>

namespace mailcore {

	class IMAPMessage : public AbstractMessage {
	private:
		uint32_t mUid;
		MessageFlag mFlags;
		MessageFlag mOriginalFlags;
		AbstractPart * mMainPart;
		Array * mLabels;
		void init();
	public:
		IMAPMessage();
		IMAPMessage(IMAPMessage * other);
		~IMAPMessage();
		
		//virtual String * className();
		virtual Object * copy();
		virtual String * description();

		virtual uint32_t uid();
		virtual void setUid(uint32_t uid);
		
		virtual void setFlags(MessageFlag flags);
		virtual MessageFlag flags();
		
		virtual void setOriginalFlags(MessageFlag flags);
		virtual MessageFlag originalFlags();
		
		virtual void setMainPart(AbstractPart * mainPart);
		virtual AbstractPart * mainPart();
		
		virtual void setGmailLabels(Array * labels);
		virtual Array * gmailLabels();
	};

}

#endif
