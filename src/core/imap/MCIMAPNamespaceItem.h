#ifndef __MAILCORE_MCIMAPNAMESPACEITEM_H

#define __MAILCORE_MCIMAPNAMESPACEITEM_H

#include <mailcore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

	class IMAPNamespaceItem : public Object {
	public:
		IMAPNamespaceItem();
		virtual ~IMAPNamespaceItem();
		
		virtual void setPrefix(String * prefix);
		virtual String * prefix();
		
		virtual void setDelimiter(char delimiter);
		virtual char delimiter();

		virtual String * pathForComponents(Array * components);
		virtual Array * componentsForPath(String * path);

		virtual bool containsFolder(String * folder);

		virtual void importIMAPNamespaceInfo(struct mailimap_namespace_info * info);
        
    public: // subclass behavior
		IMAPNamespaceItem(IMAPNamespaceItem * other);
		virtual String * description();
		virtual Object * copy();
        
	private:
	    char mDelimiter;
	    String * mPrefix;
		void init();
	};

}

#endif

#endif
