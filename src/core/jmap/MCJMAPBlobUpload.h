#ifndef MAILCORE_MCJMAPBLOBUPLOAD_H

#define MAILCORE_MCJMAPBLOBUPLOAD_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT JMAPBlobUpload : public Object {
    public:
        JMAPBlobUpload();
        virtual ~JMAPBlobUpload();

        virtual String * accountID();
        virtual void setAccountID(String * accountID);

        virtual String * blobID();
        virtual void setBlobID(String * blobID);

        virtual String * type();
        virtual void setType(String * type);

        virtual String * name();
        virtual void setName(String * name);

        virtual size_t size();
        virtual void setSize(size_t size);

    private:
        String * mAccountID;
        String * mBlobID;
        String * mType;
        String * mName;
        size_t mSize;

        void init();
    };

}

#endif

#endif
