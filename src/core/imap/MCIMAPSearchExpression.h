#ifndef __MAILCORE_MCIMAPSEARCHEXPRESSION_H_

#define __MAILCORE_MCIMAPSEARCHEXPRESSION_H_

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {

    class IMAPSearchExpression : public Object {
    public:
        IMAPSearchExpression();
        virtual ~IMAPSearchExpression();
        
        virtual IMAPSearchKind kind();
        virtual String * header();
        virtual String * value();
        virtual uint64_t longNumber();
        virtual time_t date();
        virtual IMAPSearchExpression * leftExpression();
        virtual IMAPSearchExpression * rightExpression();
        
        static IMAPSearchExpression * searchAll();
        static IMAPSearchExpression * searchFrom(String * value);
        static IMAPSearchExpression * searchRecipient(String * value);
        static IMAPSearchExpression * searchSubject(String * value);
        static IMAPSearchExpression * searchContent(String * value);
        static IMAPSearchExpression * searchHeader(String * header, String * value);
        static IMAPSearchExpression * searchRead();
        static IMAPSearchExpression * searchUnread();
        static IMAPSearchExpression * searchFlagged();
        static IMAPSearchExpression * searchUnflagged();
        static IMAPSearchExpression * searchAnswered();
        static IMAPSearchExpression * searchUnanswered();
        static IMAPSearchExpression * searchDraft();
        static IMAPSearchExpression * searchUndraft();
        static IMAPSearchExpression * searchDeleted();
        static IMAPSearchExpression * searchSpam();
        static IMAPSearchExpression * searchBeforeDate(time_t date);
        static IMAPSearchExpression * searchOnDate(time_t date);
        static IMAPSearchExpression * searchSinceDate(time_t date);
        static IMAPSearchExpression * searchBeforeReceivedDate(time_t date);
        static IMAPSearchExpression * searchOnReceivedDate(time_t date);
        static IMAPSearchExpression * searchSinceReceivedDate(time_t date);
        static IMAPSearchExpression * searchGmailThreadID(uint64_t number);
        static IMAPSearchExpression * searchAnd(IMAPSearchExpression * left, IMAPSearchExpression * right);
        static IMAPSearchExpression * searchOr(IMAPSearchExpression * left, IMAPSearchExpression * right);
        
    public: // subclass behavior
        IMAPSearchExpression(IMAPSearchExpression * other);
        virtual String * description();
        virtual Object * copy();
        
    private:
        IMAPSearchKind mKind;
        String * mHeader;
        String * mValue;
        uint64_t mLongNumber;
        time_t mDate;
        IMAPSearchExpression * mLeftExpression;
        IMAPSearchExpression * mRightExpression;
        void init();
    };
    
}

#endif

#endif
