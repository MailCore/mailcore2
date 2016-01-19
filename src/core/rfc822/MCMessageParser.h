#ifndef MAILCORE_MCPARSEDMESSAGE_H

#define MAILCORE_MCPARSEDMESSAGE_H

#include <MailCore/MCBaseTypes.h>
#include <MailCore/MCAbstractMessage.h>
#include <MailCore/MCAbstractPart.h>
#ifdef __APPLE__
#import <CoreFoundation/CoreFoundation.h>
#endif

#ifdef __cplusplus

namespace mailcore {
    
    class HTMLRendererTemplateCallback;
    
    class MAILCORE_EXPORT MessageParser : public AbstractMessage {
    public:
        static MessageParser * messageParserWithData(Data * data);
        static MessageParser * messageParserWithContentsOfFile(String * filename);
        
        MessageParser();
        MessageParser(Data * data);
        virtual ~MessageParser();
        
        virtual AbstractPart * mainPart();
        virtual Data * data();
        
        virtual String * htmlRendering(HTMLRendererTemplateCallback * htmlCallback = NULL);
        virtual String * htmlBodyRendering();
        
        virtual String * plainTextRendering();
        virtual String * plainTextBodyRendering(bool stripWhitespace);
        
    public: // subclass behavior
        MessageParser(MessageParser * other);
        virtual String * description();
        virtual Object * copy();
        
        virtual AbstractPart * partForContentID(String * contentID);
        virtual AbstractPart * partForUniqueID(String * uniqueID);
        
        virtual HashMap * serializable();
        virtual void importSerializable(HashMap * serializable);

#ifdef __APPLE__
    public:
        static MessageParser * messageParserWithData(CFDataRef data);
        MessageParser(CFDataRef data);
#endif
        
    private:
        Data * mData;
        AbstractPart * mMainPart;
        void init();
#if __APPLE__
        void * mNSData;
#endif
        
    private:
        void setBytes(char * bytes, unsigned int length);
        Data * dataFromNSData();
    };
    
};

#endif

#endif
