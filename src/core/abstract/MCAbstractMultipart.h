#ifndef __MAILCORE_MCABSTRACTMULTIPART_H

#define __MAILCORE_MCABSTRACTMULTIPART_H

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractPart.h>

namespace mailcore {
	
	class AbstractMultipart : public AbstractPart {
	private:
		Array * mParts;
		void init();
		void applyMessage();
		
	public:
		AbstractMultipart();
		AbstractMultipart(AbstractMultipart * other);
		virtual ~AbstractMultipart();
		
		virtual String * description();
		virtual Object * copy();
		
		virtual Array * parts();
		virtual void setParts(Array * parts);
		
		virtual void setMessage(AbstractMessage * message);
	};
}

#endif
