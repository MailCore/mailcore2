#ifndef __MAILCORE_IMAPMESSAGEPART_H_

#define __MAILCORE_IMAPMESSAGEPART_H_

#include <MailCore/MCAbstractMessagePart.h>

#ifdef __cplusplus

namespace mailcore {
	
	class IMAPMessagePart : public AbstractMessagePart {
	public:
		IMAPMessagePart();
		virtual ~IMAPMessagePart();
        
		virtual void setPartID(String * partID);
		virtual String * partID();
        
    public: // subclass behavior
		IMAPMessagePart(IMAPMessagePart * other);
		virtual Object * copy();
        
    private:
		String * mPartID;
		void init();
	};
}

#endif

#endif
