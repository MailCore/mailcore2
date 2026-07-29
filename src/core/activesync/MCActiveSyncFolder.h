#ifndef MAILCORE_MCACTIVESYNCFOLDER_H

#define MAILCORE_MCACTIVESYNCFOLDER_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncFolder : public Object {
    public:
        ActiveSyncFolder();
        virtual ~ActiveSyncFolder();

        virtual void setServerID(String * serverID);
        virtual String * serverID();
        virtual void setParentID(String * parentID);
        virtual String * parentID();
        virtual void setDisplayName(String * displayName);
        virtual String * displayName();

        virtual Object * copy();
        virtual String * description();

    private:
        String * mServerID;
        String * mParentID;
        String * mDisplayName;
        void init();
    };

}

#endif

#endif
