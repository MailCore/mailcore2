#ifndef MAILCORE_MCABSTRACTMESSAGE_H

#define MAILCORE_MCABSTRACTMESSAGE_H

#include <MailCore/MCBaseTypes.h>

#ifdef __cplusplus

namespace mailcore {
    
    class AbstractPart;
    class MessageHeader;
    
    class AbstractMessage : public Object {
    public:
        AbstractMessage();
        virtual ~AbstractMessage();
        
        virtual MessageHeader * header();
        virtual void setHeader(MessageHeader * header);
        
        virtual AbstractPart * partForContentID(String * contentID);
        virtual AbstractPart * partForUniqueID(String * uniqueID);
        
        virtual Array * /* AbstractPart */ attachments();
        virtual Array * /* AbstractPart */ htmlInlineAttachments();
        virtual Array * /* AbstractPart */ requiredPartsForRendering();
        
    public: //subclass behavior
        AbstractMessage(AbstractMessage * other);
        virtual String * description();
        virtual Object * copy();
        virtual HashMap * serializable();
        virtual void importSerializable(HashMap * hashmap);
        
    private:
        MessageHeader * mHeader;
        void init();
        
    };
    
}

#endif

#endif
