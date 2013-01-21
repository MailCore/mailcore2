#ifndef __MAILCORE_MCIMAPMULTIPART_H

#define __MAILCORE_MCIMAPMULTIPART_H

#include <mailcore/MCAbstractMultipart.h>

#ifdef __cplusplus

namespace mailcore {
	
	class IMAPMultipart : public AbstractMultipart {
	public:
		IMAPMultipart();
		virtual ~IMAPMultipart();
		
    public: // subclass behavior
		IMAPMultipart(IMAPMultipart * other);
		virtual Object * copy();
	};
}

#endif

#endif
