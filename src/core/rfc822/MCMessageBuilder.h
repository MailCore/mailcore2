#ifndef __MAILCORE_MCMESSAGEBUILDER_H_

#define __MAILCORE_MCMESSAGEBUILDER_H_

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractMessage.h>

namespace mailcore {
	
	class Attachment;
	
	class MessageBuilder : public AbstractMessage {
	private:
		String * mHTMLBody;
		String * mTextBody;
		Array * mAttachments;
		Array * mRelatedAttachments;
		String * mBoundaryPrefix;
		void init();
		Data * dataAndFilterBcc(bool filterBcc);
		
	public:
		MessageBuilder();
		MessageBuilder(MessageBuilder * other);
		virtual ~MessageBuilder();
		
		virtual String * description();
		//virtual String * className();
		virtual Object * copy();
		
		virtual void setHTMLBody(String * htmlBody);
		virtual String * htmlBody();
		
		virtual void setTextBody(String * textBody);
		virtual String * textBody();
		
		virtual void setAttachments(Array * /* Attachment */ attachments);
		virtual Array * /* Attachment */ attachments();
		virtual void addAttachment(Attachment * attachment);
		
		// attachments (usually images) that are included in HTML.
		virtual void setRelatedAttachments(Array * /* Attachment */ attachments);
		virtual Array * /* Attachment */ relatedAttachments();
		virtual void addRelatedAttachment(Attachment * attachment);
		
		// When boundary needs to be prefixed (to go through spam filters).
		virtual void setBoundaryPrefix(String * boundaryPrefix);
		virtual String * boundaryPrefix();
		
		virtual Data * data();
	};
	
};

#endif
