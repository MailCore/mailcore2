#ifndef MAILCORE_MCMULTIPART_H

#define MAILCORE_MCMULTIPART_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCAbstractMultipart.h>

#ifdef __cplusplus

namespace mailcore {
    
    class Multipart : public AbstractMultipart {
    public:
        Multipart();
        virtual ~Multipart();

        // Used when building messages
        // When boundary needs to be prefixed (to go through spam filters).
        virtual void setBoundaryPrefix(String * boundaryPrefix);
        virtual String * boundaryPrefix();
        
    public: // subclass behavior
        Multipart(Multipart * other);
        virtual Object * copy();
        virtual struct mailmime * mime();

    private:
        String *mBoundaryPrefix;
        void init();
    };

}

#endif

#endif
