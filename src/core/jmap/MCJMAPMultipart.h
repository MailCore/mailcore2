#ifndef MAILCORE_MCJMAPMULTIPART_H

#define MAILCORE_MCJMAPMULTIPART_H

#include <MailCore/MCAbstractMultipart.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT JMAPMultipart : public AbstractMultipart {
    public:
        JMAPMultipart();
        virtual ~JMAPMultipart();

        virtual String * partID();
        virtual void setPartID(String * partID);

        virtual String * blobID();
        virtual void setBlobID(String * blobID);

        virtual unsigned int size();
        virtual void setSize(unsigned int size);

        virtual String * language();
        virtual void setLanguage(String * language);

        virtual Array * languages();
        virtual void setLanguages(Array * languages);

    private:
        String * mPartID;
        String * mBlobID;
        unsigned int mSize;
        String * mLanguage;
        Array * mLanguages;

        void init();
    };

}

#endif

#endif
