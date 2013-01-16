#ifndef __MAILCORE_MCIMAPPART_H_

#define __MAILCORE_MCIMAPPART_H_

#include <mailcore/MCBaseTypes.h>
#include <libetpan/libetpan.h>
#include <mailcore/MCAbstractPart.h>

namespace mailcore {

	class IMAPMessagePart;
	class IMAPMultipart;

	class IMAPPart : public AbstractPart {
	private:
		String * mPartID;
		Encoding mEncoding;
		unsigned int mSize;
		void init();
		static AbstractPart * attachmentWithIMAPBodyInternal(struct mailimap_body * body, String * partID);
		static AbstractPart * attachmentWithIMAPBody1Part(struct mailimap_body_type_1part * body_1part,
			String * partID);
		static IMAPMessagePart * attachmentWithIMAPBody1PartMessage(struct mailimap_body_type_msg * message,
		    struct mailimap_body_ext_1part * extension,
		    String * partID);
		static IMAPPart * attachmentWithIMAPBody1PartBasic(struct mailimap_body_type_basic * basic,
			struct mailimap_body_ext_1part * extension);
		static IMAPPart * attachmentWithIMAPBody1PartText(struct mailimap_body_type_text * text,
			struct mailimap_body_ext_1part * extension);
		static IMAPMultipart * attachmentWithIMAPBodyMultipart(struct mailimap_body_type_mpart * body_mpart,
			String * partID);
	public:
		IMAPPart();
		IMAPPart(IMAPPart * other);
		virtual ~IMAPPart();

		virtual Object * copy();
		
		virtual void setPartID(String * partID);
		virtual String * partID();

		virtual void setSize(unsigned int size);
		virtual unsigned int size();
		
		virtual void setEncoding(Encoding encoding);
		virtual Encoding encoding();
		
		unsigned int decodedSize();
		
		static AbstractPart * attachmentWithIMAPBody(struct mailimap_body * body);
		
		virtual void importIMAPFields(struct mailimap_body_fields * fields,
			struct mailimap_body_ext_1part * extension);
	};
}

#endif
