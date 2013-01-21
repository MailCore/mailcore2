#ifndef __MAILCORE_MCMULTIPART_H

#define __MAILCORE_MCMULTIPART_H

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractMultipart.h>

#ifdef __cplusplus

namespace mailcore {
	
	class Multipart : public AbstractMultipart {
	public:
		Multipart();
		virtual ~Multipart();
		
    public: // subclass behavior
		Multipart(Multipart * other);
		virtual Object * copy();
	};
}

#endif

#endif
