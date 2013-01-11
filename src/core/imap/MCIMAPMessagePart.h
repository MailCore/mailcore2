#ifndef __MAILCORE_IMAPMESSAGEPART_H_

#define __MAILCORE_IMAPMESSAGEPART_H_

#include <mailcore/MCAbstractMessagePart.h>

namespace mailcore {
	
	class IMAPMessagePart : public AbstractMessagePart {
	private:
		
	public:
		IMAPMessagePart();
		IMAPMessagePart(IMAPMessagePart * other);
		virtual ~IMAPMessagePart();
		
		//virtual String * className();
		virtual Object * copy();
	};
}


#endif
