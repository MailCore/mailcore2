#ifndef __MAILCORE_IMAPMESSAGEPART_H_

#define __MAILCORE_IMAPMESSAGEPART_H_

#include <mailcore/MCAbstractMessagePart.h>

#ifdef __cplusplus

namespace mailcore {
	
	class IMAPMessagePart : public AbstractMessagePart {
	public:
		IMAPMessagePart();
		virtual ~IMAPMessagePart();
        
    public: // subclass behavior
		IMAPMessagePart(IMAPMessagePart * other);
		virtual Object * copy();
	};
}

#endif

#endif
