#ifndef MAILCORE_MCACTIVESYNCOPTIONS_H

#define MAILCORE_MCACTIVESYNCOPTIONS_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncOptions : public Object {
    public:
        ActiveSyncOptions();
        virtual ~ActiveSyncOptions();

        virtual void setProtocolVersions(Array * /* String */ protocolVersions);
        virtual Array * /* String */ protocolVersions();
        virtual void setCommands(Array * /* String */ commands);
        virtual Array * /* String */ commands();

        virtual Object * copy();
        virtual String * description();

    private:
        Array * /* String */ mProtocolVersions;
        Array * /* String */ mCommands;
        void init();
    };

}

#endif

#endif
