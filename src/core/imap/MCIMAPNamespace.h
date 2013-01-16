#ifndef __MAILCORE_MCIMAPNAMESPACE_H_

#define __MAILCORE_MCIMAPNAMESPACE_H_

#include <mailcore/MCBaseTypes.h>

namespace mailcore {

	class IMAPNamespaceItem;

	class IMAPNamespace : public Object {
	private:
		Array * mItems;
		void init();
		IMAPNamespaceItem * mainItem();
		IMAPNamespaceItem * itemForPath(String * path);
	public:
		IMAPNamespace();
		IMAPNamespace(IMAPNamespace * other);
		virtual ~IMAPNamespace();
		
		virtual String * description();
		virtual Object * copy();
		
		virtual String * mainPrefix();
		virtual char mainDelimiter();

		virtual Array * prefixes();

		virtual String * pathForComponents(Array * components);
		virtual String * pathForComponentsAndPrefix(Array * components, String * prefix);

		virtual Array * componentsFromPath(String * path);

		virtual bool containsFolderPath(String * path);

		static IMAPNamespace * namespaceWithPrefix(String * prefix, char delimiter);
		virtual void importIMAPNamespace(struct mailimap_namespace_item * item);
	};

}

#endif
