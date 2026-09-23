#ifndef MAILCORE_MCGMAILMESSAGEGETREQUESTPRIVATE_H

#define MAILCORE_MCGMAILMESSAGEGETREQUESTPRIVATE_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCGmailTypes.h>

namespace mailcore {

    class GmailMessageGetRequest {
    public:
        GmailMessageGetRequest();
        ~GmailMessageGetRequest();

        GmailMessageFormat format;
        Array * /* String */ metadataHeaders;
    };

}

#endif
