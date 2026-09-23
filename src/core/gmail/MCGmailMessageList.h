#ifndef MAILCORE_MCGMAILMESSAGELIST_H

#define MAILCORE_MCGMAILMESSAGELIST_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT GmailMessageList : public Object {
    public:
        GmailMessageList();
        virtual ~GmailMessageList();

        virtual Array * /* GmailMessageSummary */ messages();
        virtual void setMessages(Array * messages);

        virtual String * nextPageToken();
        virtual void setNextPageToken(String * nextPageToken);

        virtual uint32_t resultSizeEstimate();
        virtual void setResultSizeEstimate(uint32_t resultSizeEstimate);

    private:
        Array * mMessages;
        String * mNextPageToken;
        uint32_t mResultSizeEstimate;

        void init();
    };

}

#endif

#endif
