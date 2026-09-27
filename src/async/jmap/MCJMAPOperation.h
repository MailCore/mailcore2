#ifndef MAILCORE_MCJMAPOPERATION_H

#define MAILCORE_MCJMAPOPERATION_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {

    class JMAPAsyncSession;

    enum JMAPOperationKind {
        JMAPOperationKindNone,
        JMAPOperationKindConnect,
        JMAPOperationKindDiscover,
        JMAPOperationKindUpdateDraft,
        JMAPOperationKindDeleteDraft,
    };

    class MAILCORE_EXPORT JMAPOperation : public Operation {
    public:
        JMAPOperation();
        virtual ~JMAPOperation();

        virtual void setSession(JMAPAsyncSession * session);
        virtual JMAPAsyncSession * session();

        virtual void setKind(JMAPOperationKind kind);
        virtual JMAPOperationKind kind();

        virtual void setMessageID(String * messageID);
        virtual String * messageID();

        virtual void setMailboxIDs(Array * mailboxIDs);
        virtual Array * mailboxIDs();

        virtual void setKeywords(Array * keywords);
        virtual Array * keywords();

        virtual void setError(ErrorCode error);
        virtual ErrorCode error();

        virtual void start();
        virtual void main();

    private:
        JMAPAsyncSession * mSession;
        JMAPOperationKind mKind;
        String * mMessageID;
        Array * mMailboxIDs;
        Array * mKeywords;
        ErrorCode mError;
    };

}

#endif

#endif
