#ifndef __MAILCORE_MCPARSEDMESSAGE_H_

#define __MAILCORE_MCPARSEDMESSAGE_H_

#include <mailcore/MCBaseTypes.h>
#include <mailcore/MCAbstractMessage.h>
#include <mailcore/MCAbstractPart.h>

namespace mailcore {
	
	class MessageParser : public AbstractMessage {
	private:
		Data * mData;
		AbstractPart * mMainPart;
		void init();
		
	public:
		static MessageParser * messageParserWithData(Data * data);
		
		MessageParser(Data * data);
		MessageParser(MessageParser * other);
		virtual ~MessageParser();
		
		virtual String * description();
		virtual Object * copy();
		
		virtual AbstractPart * mainPart();
		virtual Data * data();
	};
	
};

#endif
