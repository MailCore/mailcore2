#ifndef __MAILCORE_IMAP_MESSAGE_H_

#define __MAILCORE_IMAP_MESSAGE_H_

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractMessage.h>
#include <mailcore/MCMessageConstants.h>
#include <mailcore/MCAbstractPart.h>

#ifdef __cplusplus

namespace mailcore {

	class IMAPMessage : public AbstractMessage {
	public:
		IMAPMessage();
		~IMAPMessage();
		
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
        
    public: // subclass behavior
		IMAPMessage(IMAPMessage * other);
		virtual Object * copy();
		virtual String * description();
        
	private:
		uint32_t mUid;
		MessageFlag mFlags;
		MessageFlag mOriginalFlags;
		AbstractPart * mMainPart;
		Array * mLabels;
		void init();
	};

}

#endif

#endif
