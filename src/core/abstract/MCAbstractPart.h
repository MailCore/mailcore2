#ifndef __MAILCORE_MCABSTRACTPART_H_

#define __MAILCORE_MCABSTRACTPART_H_

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCMessageConstants.h>

namespace mailcore {

	class AbstractMessage;

	class AbstractPart : public Object {
	private:
		String * mFilename;
		String * mMimeType;
		String * mCharset;
		String * mContentID;
		String * mContentLocation;
		bool mInlineAttachment;
		PartType mPartType;
		AbstractMessage * mMessage; // weak
		void init();
		
	public:
		AbstractPart();
		AbstractPart(AbstractPart * other);
		virtual ~AbstractPart();
		
		virtual String * description();
		virtual Object * copy();
		
		virtual PartType partType();
		virtual void setPartType(PartType type);
		
		virtual String * filename();
		virtual void setFilename(String * filename);
		
		virtual String * mimeType();
		virtual void setMimeType(String * mimeType);
		
		virtual String * charset();
		virtual void setCharset(String * charset);
		
		virtual String * contentID();
		virtual void setContentID(String * contentID);
		
		virtual String * contentLocation();
		virtual void setContentLocation(String * contentLocation);
		
		virtual bool isInlineAttachment();
		virtual void setInlineAttachment(bool inlineAttachment);
		
		virtual AbstractMessage * message();
		virtual void setMessage(AbstractMessage * message);
		
		virtual void importIMAPFields(struct mailimap_body_fields * fields,
			struct mailimap_body_ext_1part * extension);
	};
	
}

#endif
