#ifndef __MAILCORE_MCABSTRACTPART_H_

#define __MAILCORE_MCABSTRACTPART_H_

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {

	class AbstractMessage;

	class AbstractPart : public Object {
	public:
		AbstractPart();
		virtual ~AbstractPart();
		
		virtual PartType partType();
		virtual void setPartType(PartType type);
		
		virtual String * filename();
		virtual void setFilename(String * filename);
		
		virtual String * mimeType();
		virtual void setMimeType(String * mimeType);
		
		virtual String * charset();
		virtual void setCharset(String * charset);
		
		virtual String * uniqueID();
		virtual void setUniqueID(String * uniqueID);
        
		virtual String * contentID();
		virtual void setContentID(String * contentID);
		
		virtual String * contentLocation();
		virtual void setContentLocation(String * contentLocation);
		
		virtual bool isInlineAttachment();
		virtual void setInlineAttachment(bool inlineAttachment);
		
        virtual AbstractPart * partForContentID(String * contentID);
        virtual AbstractPart * partForUniqueID(String * uniqueID);
        
		virtual String * decodedStringForData(Data * data);
		
    public: // subclass behavior
		AbstractPart(AbstractPart * other);
		virtual String * description();
		virtual Object * copy();
        
    public: // private
		virtual void importIMAPFields(struct mailimap_body_fields * fields,
                                      struct mailimap_body_ext_1part * extension);
        virtual void applyUniquePartID();
        
	private:
		String * mUniqueID;
		String * mFilename;
		String * mMimeType;
		String * mCharset;
		String * mContentID;
		String * mContentLocation;
		bool mInlineAttachment;
		PartType mPartType;
		void init();
	};
	
}

#endif

#endif
