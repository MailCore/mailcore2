#ifndef MAILCORE_MCJMAPMESSAGE_H

#define MAILCORE_MCJMAPMESSAGE_H

#include <MailCore/MCAbstractMessage.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT JMAPMessage : public AbstractMessage {
    public:
        JMAPMessage();
        virtual ~JMAPMessage();

        virtual String * identifier();
        virtual void setIdentifier(String * identifier);

        virtual String * blobID();
        virtual void setBlobID(String * blobID);

        virtual String * threadID();
        virtual void setThreadID(String * threadID);

        virtual Array * mailboxIDs();
        virtual void setMailboxIDs(Array * mailboxIDs);

        virtual Array * keywords();
        virtual void setKeywords(Array * keywords);

        virtual unsigned int size();
        virtual void setSize(unsigned int size);

        virtual String * preview();
        virtual void setPreview(String * preview);

        virtual AbstractPart * mainPart();
        virtual void setMainPart(AbstractPart * mainPart);

        virtual Array * textBody();
        virtual void setTextBody(Array * textBody);

        virtual Array * htmlBody();
        virtual void setHTMLBody(Array * htmlBody);

        virtual Array * attachments();
        virtual void setAttachments(Array * attachments);

        virtual AbstractPart * partForPartID(String * partID);
        virtual AbstractPart * partForContentID(String * contentID);
        virtual AbstractPart * partForUniqueID(String * uniqueID);

    private:
        String * mIdentifier;
        String * mBlobID;
        String * mThreadID;
        Array * mMailboxIDs;
        Array * mKeywords;
        unsigned int mSize;
        String * mPreview;
        AbstractPart * mMainPart;
        Array * mTextBody;
        Array * mHTMLBody;
        Array * mAttachments;

        void init();
    };

}

#endif

#endif
