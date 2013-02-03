#ifndef __MAILCORE_MCABSTRACTMULTIPART_H

#define __MAILCORE_MCABSTRACTMULTIPART_H

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractPart.h>

#ifdef __cplusplus

namespace mailcore {
	
	class AbstractMultipart : public AbstractPart {
	public:
		AbstractMultipart();
		virtual ~AbstractMultipart();
		
		virtual Array * parts();
		virtual void setParts(Array * parts);
		
		virtual void setMessage(AbstractMessage * message);
        
        virtual AbstractPart * partForContentID(String * contentID);
        virtual AbstractPart * partForUniqueID(String * uniqueID);
        
    public: //subclass behavior
		AbstractMultipart(AbstractMultipart * other);
		virtual String * description();
		virtual Object * copy();
		
	private:
		Array * mParts;
		void init();
		void applyMessage();
	};
}

#endif

#endif
