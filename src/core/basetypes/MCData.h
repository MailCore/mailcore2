#ifndef __MAILCORE_MCDATA_H_

#define __MAILCORE_MCDATA_H_

#include <MailCore/MCObject.h>
#include <MailCore/MCMessageConstants.h>

#ifdef __cplusplus

namespace mailcore {

	class String;

	class Data : public Object {
	public:
		Data();
		Data(int capacity);
		Data(const char * bytes, unsigned int length);
		virtual ~Data();
		
		static Data * data();
		static Data * dataWithCapacity(int capacity);
		static Data * dataWithContentsOfFile(String * filename);
		static Data * dataWithBytes(const char * bytes, unsigned int length);
		
		virtual char * bytes();
		virtual unsigned int length();
		
		virtual void appendData(Data * otherData);
		virtual void appendBytes(const char * bytes, unsigned int length);
		virtual void setBytes(const char * bytes, unsigned int length);
		virtual void setData(Data * otherData);
		
		// Helpers
		virtual String * stringWithDetectedCharset();
		virtual String * stringWithDetectedCharset(String * charset, bool isHTML);
		virtual String * stringWithCharset(const char * charset);
		virtual Data * decodedDataUsingEncoding(Encoding encoding);
        
    public: // private
		virtual String * charsetWithFilteredHTML(bool filterHTML, String * hintCharset = NULL);
        
    public: // subclass behavior
		Data(Data * otherData);
		virtual String * description();
		virtual Object * copy();
		virtual bool isEqual(Object * otherObject);
		virtual unsigned int hash();
        
	private:
		char * mBytes;
		unsigned int mLength;
		unsigned int mAllocated;
		void allocate(unsigned int length);
		void reset();
		static String * normalizeCharset(String * charset);
		String * charsetWithFilteredHTMLWithoutHint(bool filterHTML);
		
	};

}

#endif

#endif
