#ifndef __MAILCORE_MCIMAPMULTIPART_H

#define __MAILCORE_MCIMAPMULTIPART_H

#include <mailcore/MCAbstractMultipart.h>

namespace mailcore {
	
	class IMAPMultipart : public AbstractMultipart {
	private:
		
	public:
		IMAPMultipart();
		IMAPMultipart(IMAPMultipart * other);
		virtual ~IMAPMultipart();
		
		//virtual String * className();
		virtual Object * copy();
	};
}

#endif
