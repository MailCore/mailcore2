#ifndef __MAILCORE_MCMULTIPART_H

#define __MAILCORE_MCMULTIPART_H

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractMultipart.h>

namespace mailcore {
	
	class Multipart : public AbstractMultipart {
	public:
		Multipart();
		Multipart(Multipart * other);
		virtual ~Multipart();
		
		virtual Object * copy();
	};
}

#endif
