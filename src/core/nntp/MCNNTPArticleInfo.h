#ifndef MAILCORE_MCNNTPARTICLEINFO_H

#define MAILCORE_MCNNTPARTICLEINFO_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {
    
    class NNTPArticleInfo : public Object {
    public:
        NNTPArticleInfo();
        virtual ~NNTPArticleInfo();
        
        virtual void setIndex(unsigned int index);
        virtual unsigned int index();
        
        virtual void setSize(unsigned int size);
        virtual unsigned int size();
        
        virtual void setUid(String * uid);
        virtual String * uid();
        
    public: // subclass behavior
        NNTPArticleInfo(NNTPArticleInfo * other);
        virtual String * description();
        virtual Object * copy();
        
    private:
        unsigned int mIndex;
        unsigned int mSize;
        String * mUid;
        
        void init();
    };
    
}

#endif

#endif
