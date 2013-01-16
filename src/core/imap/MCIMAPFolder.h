#ifndef __MAILCORE_MCIMAPFOLDER_H

#define __MAILCORE_MCIMAPFOLDER_H

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCMessageConstants.h>

namespace mailcore {
	
	class IMAPFolder : public Object {
	private:
		String * mPath;
		char mDelimiter;
		IMAPFolderFlag mFlags;
		void init();
	public:
		IMAPFolder();
		IMAPFolder(IMAPFolder * other);
		virtual ~IMAPFolder();
		
		virtual Object * copy();
		
		virtual void setPath(String * path);
		virtual String * path();
		
		virtual void setDelimiter(char delimiter);
		virtual char delimiter();
		
		virtual void setFlags(IMAPFolderFlag flags);
		virtual IMAPFolderFlag flags();
	};
	
}

#endif
