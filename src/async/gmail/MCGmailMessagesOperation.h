#ifndef MAILCORE_MCGMAILMESSAGESOPERATION_H

#define MAILCORE_MCGMAILMESSAGESOPERATION_H

#include <MailCore/MCGmailOperation.h>

#ifdef __cplusplus

namespace mailcore {

    class GmailMessageList;

    enum GmailMessagesOperationKind {
        GmailMessagesOperationKindDefault,
        GmailMessagesOperationKindQuery,
        GmailMessagesOperationKindLabel,
    };

    class MAILCORE_EXPORT GmailMessagesOperation : public GmailOperation {
    public:
        GmailMessagesOperation();
        virtual ~GmailMessagesOperation();

        virtual void setKind(GmailMessagesOperationKind kind);
        virtual GmailMessagesOperationKind kind();
        virtual void setQuery(String * query);
        virtual String * query();
        virtual void setLabelID(String * labelID);
        virtual String * labelID();
        virtual GmailMessageList * messages();
        virtual void main();

    private:
        GmailMessagesOperationKind mKind;
        String * mQuery;
        String * mLabelID;
        GmailMessageList * mMessages;
    };

}

#endif

#endif
