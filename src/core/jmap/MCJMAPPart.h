#ifndef MAILCORE_MCJMAPPART_H

#define MAILCORE_MCJMAPPART_H

#include <MailCore/MCAbstractPart.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT JMAPPart : public AbstractPart {
    public:
        JMAPPart();
        virtual ~JMAPPart();

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
