#ifndef MAILCORE_MCGMAILMESSAGELISTREQUESTPRIVATE_H

#define MAILCORE_MCGMAILMESSAGELISTREQUESTPRIVATE_H

#include <MailCore/MCBaseTypes.h>

namespace mailcore {

    class GmailMessageListRequest {
    public:
        GmailMessageListRequest();
        ~GmailMessageListRequest();

        String * query;
        Array * /* String */ labelIDs;
        uint32_t maxResults;
        String * pageToken;
        bool includeSpamTrash;
    };

}

#endif
