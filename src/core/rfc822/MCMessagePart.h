#ifndef __MAILCORE_MCMESSAGEPART_H_

#define __MAILCORE_MCMESSAGEPART_H_

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCAbstractMessagePart.h>

#ifdef __cplusplus

namespace mailcore {
	
	class MessagePart : public AbstractMessagePart {
	public:
		MessagePart();
		virtual ~MessagePart();
        
    public: // subclass behavior
		MessagePart(MessagePart * other);
		virtual Object * copy();
	};
}

#endif

#endif
