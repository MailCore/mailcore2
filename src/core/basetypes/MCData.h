#ifndef __MAILCORE_MCDATA_H_

#define __MAILCORE_MCDATA_H_

#include <mailcore/MCObject.h>
#include <mailcore/MCMessageConstants.h>

namespace mailcore {

	class String;

	class Data : public Object {
	private:
		char * mBytes;
		unsigned int mLength;
		unsigned int mAllocated;
		void allocate(unsigned int length);
		void reset();
		static String * normalizeCharset(String * charset);
		String * charsetWithFilteredHTMLWithoutHint(bool filterHTML);
		
	public:
		Data();
		Data(int capacity);
		Data(Data * otherData);
		Data(const char * bytes, unsigned int length);
		virtual ~Data();
		
		virtual String * description();
		virtual Object * copy();
		virtual bool isEqual(Object * otherObject);
		virtual unsigned int hash();
		
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
		virtual String * emailStringWithCharset(String * charset);
		virtual String * stringWithCharset(const char * charset);
		virtual String * charsetWithFilteredHTML(bool filterHTML, String * hintCharset = NULL);
		virtual Data * decodedDataUsingEncoding(Encoding encoding);
	};

}

#endif
