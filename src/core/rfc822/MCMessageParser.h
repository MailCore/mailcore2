#ifndef __MAILCORE_MCPARSEDMESSAGE_H_

#define __MAILCORE_MCPARSEDMESSAGE_H_

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCAbstractMessage.h>
#include <MailCore/MCAbstractPart.h>

#ifdef __cplusplus

namespace mailcore {
	
    class HTMLRendererTemplateCallback;
    
	class MessageParser : public AbstractMessage {
	public:
		static MessageParser * messageParserWithData(Data * data);
		static MessageParser * messageParserWithContentsOfFile(String * filename);
		
		MessageParser(Data * data);
		virtual ~MessageParser();
		
		virtual AbstractPart * mainPart();
		virtual Data * data();
        
        virtual String * htmlRendering(HTMLRendererTemplateCallback * htmlCallback = NULL);
        virtual String * htmlBodyRendering();
        
        virtual String * plainTextRendering();
        virtual String * plainTextBodyRendering();
        
    public: // subclass behavior
		MessageParser(MessageParser * other);
		virtual String * description();
		virtual Object * copy();
        
        virtual AbstractPart * partForContentID(String * contentID);
        virtual AbstractPart * partForUniqueID(String * uniqueID);
        
	private:
		Data * mData;
		AbstractPart * mMainPart;
		void init();
		
	};
	
};

#endif

#endif
