#ifndef MAILCORE_MCACTIVESYNCMOVERESPONSE_H

#define MAILCORE_MCACTIVESYNCMOVERESPONSE_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCActiveSyncTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncMoveResponse : public Object {
    public:
        ActiveSyncMoveResponse();
        virtual ~ActiveSyncMoveResponse();

        virtual void setSourceMessageID(String * sourceMessageID);
        virtual String * sourceMessageID();
        virtual void setSourceFolderID(String * sourceFolderID);
        virtual String * sourceFolderID();
        virtual void setDestinationFolderID(String * destinationFolderID);
        virtual String * destinationFolderID();
        virtual void setDestinationMessageID(String * destinationMessageID);
        virtual String * destinationMessageID();
        virtual void setStatus(ActiveSyncMoveStatus status);
        virtual ActiveSyncMoveStatus status();

        virtual Object * copy();
        virtual String * description();

    private:
        String * mSourceMessageID;
        String * mSourceFolderID;
        String * mDestinationFolderID;
        String * mDestinationMessageID;
        ActiveSyncMoveStatus mStatus;
        void init();
    };

}

#endif

#endif
