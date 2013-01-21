#ifndef __MAILCORE_MCABSTRACTMESSAGE_H_
#define __MAILCORE_MCABSTRACTMESSAGE_H_

#include <mailcore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

	class MessageHeader;

	class AbstractMessage : public Object {
	public:
		AbstractMessage();
		virtual ~AbstractMessage();

		virtual MessageHeader * header();
		virtual void setHeader(MessageHeader * header);
        
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
