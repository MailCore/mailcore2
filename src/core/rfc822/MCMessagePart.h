#ifndef __MAILCORE_MCMESSAGEPART_H_

#define __MAILCORE_MCMESSAGEPART_H_

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractMessagePart.h>

namespace mailcore {
	
	class MessagePart : public AbstractMessagePart {
	public:
		MessagePart();
		MessagePart(MessagePart * other);
		virtual ~MessagePart();
		
		virtual Object * copy();
	};
}


#endif
