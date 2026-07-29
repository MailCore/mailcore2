#ifndef MAILCORE_MCACTIVESYNCMOVE_H

#define MAILCORE_MCACTIVESYNCMOVE_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncMove : public Object {
    public:
        ActiveSyncMove();
        virtual ~ActiveSyncMove();

        virtual void setSourceMessageID(String * sourceMessageID);
        virtual String * sourceMessageID();
        virtual void setSourceFolderID(String * sourceFolderID);
        virtual String * sourceFolderID();
        virtual void setDestinationFolderID(String * destinationFolderID);
        virtual String * destinationFolderID();

        virtual Object * copy();
        virtual String * description();

    private:
        String * mSourceMessageID;
        String * mSourceFolderID;
        String * mDestinationFolderID;
        void init();
    };

}

#endif

#endif
