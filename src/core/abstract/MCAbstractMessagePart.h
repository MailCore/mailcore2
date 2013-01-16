#ifndef __MAILCORE_MCABSTRACTMESSAGEPART_H_

#define __MAILCORE_MCABSTRACTMESSAGEPART_H_

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractPart.h>

namespace mailcore {
	
	class MessageHeader;
	
	class AbstractMessagePart : public AbstractPart {
	private:
		AbstractPart * mMainPart;
		MessageHeader * mHeader;
		void init();
		void applyMessage();
		
	public:
		AbstractMessagePart();
		AbstractMessagePart(AbstractMessagePart * other);
		virtual ~AbstractMessagePart();
		
		virtual String * description();
		virtual Object * copy();
		
		virtual MessageHeader * header();
		virtual void setHeader(MessageHeader * header);
		
		virtual AbstractPart * mainPart();
		virtual void setMainPart(AbstractPart * mainPart);
		
		virtual void setMessage(AbstractMessage * message);
	};
}


#endif
