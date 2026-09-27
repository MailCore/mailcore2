#ifndef MAILCORE_MCJMAPSUBMISSION_H

#define MAILCORE_MCJMAPSUBMISSION_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT JMAPSubmission : public Object {
    public:
        JMAPSubmission();
        virtual ~JMAPSubmission();

        virtual String * identifier();
        virtual void setIdentifier(String * identifier);

        virtual String * emailID();
        virtual void setEmailID(String * emailID);

        virtual String * threadID();
        virtual void setThreadID(String * threadID);

        virtual String * identityID();
        virtual void setIdentityID(String * identityID);

        virtual String * sendAt();
        virtual void setSendAt(String * sendAt);

        virtual String * undoStatus();
        virtual void setUndoStatus(String * undoStatus);

        virtual HashMap * deliveryStatus();
        virtual void setDeliveryStatus(HashMap * deliveryStatus);

    private:
        String * mIdentifier;
        String * mEmailID;
        String * mThreadID;
        String * mIdentityID;
        String * mSendAt;
        String * mUndoStatus;
        HashMap * mDeliveryStatus;

        void init();
    };

}

#endif

#endif
