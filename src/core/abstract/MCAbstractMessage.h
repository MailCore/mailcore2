#ifndef __MAILCORE_MCABSTRACTMESSAGE_H_
#define __MAILCORE_MCABSTRACTMESSAGE_H_

#include <mailcore/MCBaseTypes.h>

namespace mailcore {

	class MessageHeader;

	class AbstractMessage : public Object {
	private:
		MessageHeader * mHeader;
		void init();

	public:
		AbstractMessage();
		AbstractMessage(AbstractMessage * other);
		virtual ~AbstractMessage();

		virtual String * description();
		virtual Object * copy();

		virtual MessageHeader * header();
		virtual void setHeader(MessageHeader * header);
	};
}

#endif
