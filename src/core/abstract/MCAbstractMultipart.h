#ifndef __MAILCORE_MCABSTRACTMULTIPART_H

#define __MAILCORE_MCABSTRACTMULTIPART_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCAbstractPart.h>

#ifdef __cplusplus

namespace mailcore {
	
	class AbstractMultipart : public AbstractPart {
	public:
		AbstractMultipart();
		virtual ~AbstractMultipart();
		
		virtual Array * parts();
		virtual void setParts(Array * parts);
		
    public: //subclass behavior
		AbstractMultipart(AbstractMultipart * other);
		virtual String * description();
		virtual Object * copy();
        
        virtual AbstractPart * partForContentID(String * contentID);
        virtual AbstractPart * partForUniqueID(String * uniqueID);
		
	private:
		Array * mParts;
		void init();
	};
}

#endif

#endif
