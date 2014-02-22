#ifndef MAILCORE_MCIMAPNAMESPACEITEM_H

#define MAILCORE_MCIMAPNAMESPACEITEM_H

#include <MailCore/MCBaseTypes.h>

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
        virtual Array * /* String */ componentsForPath(String * path);
        
        virtual bool containsFolder(String * folder);
        
    public: // subclass behavior
        IMAPNamespaceItem(IMAPNamespaceItem * other);
        virtual String * description();
        virtual Object * copy();
        
    private:
        char mDelimiter;
        String * mPrefix;
        void init();
        
    public: // private
        virtual void importIMAPNamespaceInfo(struct mailimap_namespace_info * info);
    
    };
    
}

#endif

#endif
