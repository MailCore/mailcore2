#ifndef __MAILCORE_MCPARSEDMESSAGE_H_

#define __MAILCORE_MCPARSEDMESSAGE_H_

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractMessage.h>
#include <mailcore/MCAbstractPart.h>

#ifdef __cplusplus

namespace mailcore {
	
	class MessageParser : public AbstractMessage {
	public:
		static MessageParser * messageParserWithData(Data * data);
		
		MessageParser(Data * data);
		virtual ~MessageParser();
		
		virtual AbstractPart * mainPart();
		virtual Data * data();
        
    public: // subclass behavior
		MessageParser(MessageParser * other);
		virtual String * description();
		virtual Object * copy();
        
	private:
		Data * mData;
		AbstractPart * mMainPart;
		void init();
		
	};
	
};

#endif

#endif
