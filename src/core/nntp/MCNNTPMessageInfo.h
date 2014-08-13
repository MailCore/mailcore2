#ifndef MAILCORE_MCNNTPMESSAGEINFO_H

#define MAILCORE_MCNNTPMESSAGEINFO_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {
    
    class NNTPMessageInfo : public Object {
    public:
        NNTPMessageInfo();
        virtual ~NNTPMessageInfo();
        
        virtual void setIndex(unsigned int index);
        virtual unsigned int index();
        
        virtual void setSize(unsigned int size);
        virtual unsigned int size();
        
        virtual void setUid(String * uid);
        virtual String * uid();
        
    public: // subclass behavior
        NNTPMessageInfo(NNTPMessageInfo * other);
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
