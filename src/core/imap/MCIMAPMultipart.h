#ifndef __MAILCORE_MCIMAPMULTIPART_H

#define __MAILCORE_MCIMAPMULTIPART_H

#include <MailCore/MCAbstractMultipart.h>

#ifdef __cplusplus

namespace mailcore {
	
	class IMAPMultipart : public AbstractMultipart {
	public:
		IMAPMultipart();
		virtual ~IMAPMultipart();
		
		virtual void setPartID(String * partID);
		virtual String * partID();
        
    public: // subclass behavior
		IMAPMultipart(IMAPMultipart * other);
		virtual Object * copy();
        
    private:
		String * mPartID;
		void init();
	};
}

#endif

#endif
