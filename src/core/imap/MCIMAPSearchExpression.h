#ifndef __MAILCORE_MCIMAPSEARCHEXPRESSION_H_

#define __MAILCORE_MCIMAPSEARCHEXPRESSION_H_

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {

	class IMAPSearchExpression : public Object {
	public:
		IMAPSearchExpression();
        IMAPSearchExpression(bool shouldAvoidCharset);
		virtual ~IMAPSearchExpression();
		
		virtual IMAPSearchKind kind();
		virtual String * header();
		virtual String * value();
        virtual bool shouldAvoidCharset();
		virtual IMAPSearchExpression * leftExpression();
		virtual IMAPSearchExpression * rightExpression();
		
		static IMAPSearchExpression * searchFrom(String * value);
		static IMAPSearchExpression * searchRecipient(String * value);
		static IMAPSearchExpression * searchSubject(String * value);
		static IMAPSearchExpression * searchContent(String * value);
		static IMAPSearchExpression * searchHeader(String * header, String * value);
		static IMAPSearchExpression * searchAnd(IMAPSearchExpression * left, IMAPSearchExpression * right);
		static IMAPSearchExpression * searchOr(IMAPSearchExpression * left, IMAPSearchExpression * right);
        
        // charset-controled methods
		static IMAPSearchExpression * searchFrom(String * value, bool shouldAvoidCharset);
		static IMAPSearchExpression * searchRecipient(String * value, bool shouldAvoidCharset);
		static IMAPSearchExpression * searchSubject(String * value, bool shouldAvoidCharset);
		static IMAPSearchExpression * searchContent(String * value, bool shouldAvoidCharset);
		static IMAPSearchExpression * searchHeader(String * header, String * value, bool shouldAvoidCharset);
		static IMAPSearchExpression * searchAnd(IMAPSearchExpression * left, IMAPSearchExpression * right, bool shouldAvoidCharset);
		static IMAPSearchExpression * searchOr(IMAPSearchExpression * left, IMAPSearchExpression * right, bool shouldAvoidCharset);
        
    public: // subclass behavior
		IMAPSearchExpression(IMAPSearchExpression * other);
		virtual String * description();
		virtual Object * copy();
        
	private:
        bool mShouldAvoidCharset;
		IMAPSearchKind mKind;
		String * mHeader;
		String * mValue;
		IMAPSearchExpression * mLeftExpression;
		IMAPSearchExpression * mRightExpression;
		void init();
	};
	
}

#endif

#endif
