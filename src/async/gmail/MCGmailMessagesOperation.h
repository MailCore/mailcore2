#ifndef MAILCORE_MCGMAILMESSAGESOPERATION_H

#define MAILCORE_MCGMAILMESSAGESOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class GmailMessageList;

    class MAILCORE_EXPORT GmailMessagesOperation : public GmailOperation {
        friend class GmailAsyncSession;

    public:
        GmailMessagesOperation();
        virtual ~GmailMessagesOperation();

        virtual String * query();
        virtual String * labelID();
        virtual GmailMessageList * messages();
        virtual void main();

    private:
        enum GmailMessagesOperationKind {
            GmailMessagesOperationKindDefault,
            GmailMessagesOperationKindQuery,
            GmailMessagesOperationKindLabel,
        };

        virtual void setKind(GmailMessagesOperationKind kind);
        virtual GmailMessagesOperationKind kind();
        virtual void setQuery(String * query);
        virtual void setLabelID(String * labelID);

        GmailMessagesOperationKind mKind;
        String * mQuery;
        String * mLabelID;
        GmailMessageList * mMessages;
    };

}

#endif

#endif
