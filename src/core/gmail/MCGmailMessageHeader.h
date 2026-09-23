#ifndef MAILCORE_MCGMAILMESSAGEHEADER_H

#define MAILCORE_MCGMAILMESSAGEHEADER_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailMessageHeader : public Object {
    public:
        GmailMessageHeader();
        virtual ~GmailMessageHeader();

        virtual String * name();
        virtual void setName(String * name);

        virtual String * value();
        virtual void setValue(String * value);

    private:
        String * mName;
        String * mValue;

        void init();
    };

}

#endif

#endif
