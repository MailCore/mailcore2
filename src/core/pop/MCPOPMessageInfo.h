#ifndef __MAILCORE_MCPOPMESSAGEINFO_H_

#define __MAILCORE_MCPOPMESSAGEINFO_H_

#include <mailcore/MCBaseTypes.h>

namespace mailcore {

	class POPMessageInfo : public Object {
	private:
		unsigned int mIndex;
		unsigned int mSize;
		String * mUid;
		
		void init();
		
	public:
		POPMessageInfo();
		POPMessageInfo(POPMessageInfo * other);
		virtual ~POPMessageInfo();
		
		//virtual String * className();
		virtual String * description();
		virtual Object * copy();
		
		virtual void setIndex(unsigned int index);
		virtual unsigned int index();
		
		virtual void setSize(unsigned int size);
		virtual unsigned int size();
		
		virtual void setUid(String * uid);
		virtual String * uid();
	};

}

#endif
