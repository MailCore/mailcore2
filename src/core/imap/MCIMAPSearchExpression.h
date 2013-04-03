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
		virtual IMAPSearchExpression * leftExpression();
		virtual IMAPSearchExpression * rightExpression();
		
		static IMAPSearchExpression * searchFrom(String * value);
		static IMAPSearchExpression * searchRecipient(String * value);
		static IMAPSearchExpression * searchSubject(String * value);
		static IMAPSearchExpression * searchContent(String * value);
		static IMAPSearchExpression * searchHeader(String * header, String * value);
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
		IMAPSearchExpression * mLeftExpression;
		IMAPSearchExpression * mRightExpression;
		void init();
	};
	
}

#endif

#endif
