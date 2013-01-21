#ifndef __MAILCORE_MCABSTRACTMESSAGEPART_H_

#define __MAILCORE_MCABSTRACTMESSAGEPART_H_

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractPart.h>

#ifdef __cplusplus

namespace mailcore {
	
	class MessageHeader;
	
	class AbstractMessagePart : public AbstractPart {
	public:
		AbstractMessagePart();
		virtual ~AbstractMessagePart();
		
		virtual MessageHeader * header();
		virtual void setHeader(MessageHeader * header);
		
		virtual AbstractPart * mainPart();
		virtual void setMainPart(AbstractPart * mainPart);
		
		virtual void setMessage(AbstractMessage * message);
        
    public: //subclass behavior
		AbstractMessagePart(AbstractMessagePart * other);
		virtual String * description();
		virtual Object * copy();
        
	private:
		AbstractPart * mMainPart;
		MessageHeader * mHeader;
		void init();
		void applyMessage();
	};
}

#endif

#endif
