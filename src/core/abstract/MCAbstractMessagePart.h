#ifndef __MAILCORE_MCABSTRACTMESSAGEPART_H_

#define __MAILCORE_MCABSTRACTMESSAGEPART_H_

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCAbstractPart.h>

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
		
    public: //subclass behavior
		AbstractMessagePart(AbstractMessagePart * other);
		virtual String * description();
		virtual Object * copy();
        
        virtual AbstractPart * partForContentID(String * contentID);
        virtual AbstractPart * partForUniqueID(String * uniqueID);
        
	private:
		AbstractPart * mMainPart;
		MessageHeader * mHeader;
		void init();
	};
}

#endif

#endif
