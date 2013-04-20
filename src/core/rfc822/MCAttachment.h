#ifndef __MAILCORE_MCATTACHMENT_H_

#define __MAILCORE_MCATTACHMENT_H_

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCAbstractPart.h>
#include <MailCore/MCAbstractMultipart.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {
	
	class MessagePart;
	
	class Attachment : public AbstractPart {
	public:
		static String * mimeTypeForFilename(String * filename);
		static Attachment * attachmentWithContentsOfFile(String * filename);
		static Attachment * attachmentWithHTMLString(String * htmlString);
		static Attachment * attachmentWithRFC822Message(Data * messageData);
		static Attachment * attachmentWithText(String * text);
		
		Attachment();
		virtual ~Attachment();
		
		virtual void setData(Data * data);
		virtual Data * data();
		virtual String * decodedString();
		
    public: // subclass behavior
		Attachment(Attachment * other);
		virtual String * description();
		virtual Object * copy();
        
    public: // private
		static AbstractPart * attachmentsWithMIME(struct mailmime * mime);
        
	private:
		Data * mData;
		void init();
		static void fillMultipartSubAttachments(AbstractMultipart * multipart, struct mailmime * mime);
		static AbstractPart * attachmentsWithMIMEWithMain(struct mailmime * mime, bool isMain);
		static Attachment * attachmentWithSingleMIME(struct mailmime * mime);
		static MessagePart * attachmentWithMessageMIME(struct mailmime * mime);
		static Encoding encodingForMIMEEncoding(struct mailmime_mechanism * mechanism, int defaultMimeEncoding);
        static HashMap * readMimeTypesFile(String * filename);
	};
	
}

#endif

#endif
