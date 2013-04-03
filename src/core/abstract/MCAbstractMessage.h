#ifndef __MAILCORE_MCABSTRACTMESSAGE_H_
#define __MAILCORE_MCABSTRACTMESSAGE_H_

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class AbstractPart;
	class MessageHeader;

	class AbstractMessage : public Object {
	public:
		AbstractMessage();
		virtual ~AbstractMessage();

		virtual MessageHeader * header();
		virtual void setHeader(MessageHeader * header);
        
        virtual AbstractPart * partForContentID(String * contentID);
        virtual AbstractPart * partForUniqueID(String * uniqueID);
        
    public: //subclass behavior
		AbstractMessage(AbstractMessage * other);
		virtual String * description();
		virtual Object * copy();
        
	private:
		MessageHeader * mHeader;
		void init();
        
	};
}

#endif

#endif
