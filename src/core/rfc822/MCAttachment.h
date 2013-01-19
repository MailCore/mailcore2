#ifndef __MAILCORE_MCATTACHMENT_H_

#define __MAILCORE_MCATTACHMENT_H_

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractPart.h>
#include <mailcore/MCAbstractMultipart.h>
#include <mailcore/MCMessageConstants.h>

namespace mailcore {
	
	class MessagePart;
	
	class Attachment : public AbstractPart {
	private:
		Data * mData;
		void init();
		static void fillMultipartSubAttachments(AbstractMultipart * multipart, struct mailmime * mime);
		static AbstractPart * attachmentsWithMIMEWithMain(struct mailmime * mime, bool isMain);
		static Attachment * attachmentWithSingleMIME(struct mailmime * mime);
		static MessagePart * attachmentWithMessageMIME(struct mailmime * mime);
		static Encoding encodingForMIMEEncoding(struct mailmime_mechanism * mechanism, int defaultMimeEncoding);
        static HashMap * readMimeTypesFile(String * filename);

	public:
		static String * mimeTypeForFilename(String * filename);
		static Attachment * attachmentWithContentOfFile(String * filename);
		static Attachment * attachmentWithHTMLString(String * htmlString);
		static Attachment * attachmentWithRFC822Message(Data * messageData);
		static Attachment * attachmentWithText(String * text);
		
		Attachment();
		Attachment(Attachment * other);
		virtual ~Attachment();
		
		virtual String * description();
		virtual Object * copy();
		
		virtual void setData(Data * data);
		virtual Data * data();
		
		static AbstractPart * attachmentsWithMIME(struct mailmime * mime);
	};
	
}

#endif
