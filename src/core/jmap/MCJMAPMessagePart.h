#ifndef MAILCORE_MCJMAPMESSAGEPART_H

#define MAILCORE_MCJMAPMESSAGEPART_H

#include <MailCore/MCAbstractMessagePart.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT JMAPMessagePart : public AbstractMessagePart {
    public:
        JMAPMessagePart();
        virtual ~JMAPMessagePart();

        virtual String * partID();
        virtual void setPartID(String * partID);

        virtual String * blobID();
        virtual void setBlobID(String * blobID);

        virtual unsigned int size();
        virtual void setSize(unsigned int size);

    private:
        String * mPartID;
        String * mBlobID;
        unsigned int mSize;

        void init();
    };

}

#endif

#endif
