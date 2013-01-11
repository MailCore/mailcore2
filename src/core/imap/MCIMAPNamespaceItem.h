#ifndef __MAILCORE_MCIMAPNAMESPACEITEM_H

#define __MAILCORE_MCIMAPNAMESPACEITEM_H

#include <mailcore/MCBaseTypes.h>

namespace mailcore {

	class IMAPNamespaceItem : public Object {
	private:
	    char mDelimiter;
	    String * mPrefix;
		void init();
	public:
		IMAPNamespaceItem();
		IMAPNamespaceItem(IMAPNamespaceItem * other);
		virtual ~IMAPNamespaceItem();
		
		//virtual String * className();
		virtual String * description();
		virtual Object * copy();
		
		virtual void setPrefix(String * prefix);
		virtual String * prefix();
		
		virtual void setDelimiter(char delimiter);
		virtual char delimiter();

		virtual String * pathForComponents(Array * components);
		virtual Array * componentsForPath(String * path);

		virtual bool containsFolder(String * folder);

		virtual void importIMAPNamespaceInfo(struct mailimap_namespace_info * info);
	};

}

#endif
