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
        
    public: // subclass behavior
        Multipart(Multipart * other);
        virtual Object * copy();
    };
    
}

#endif

#endif
