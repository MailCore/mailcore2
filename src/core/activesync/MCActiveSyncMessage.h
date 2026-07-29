#ifndef MAILCORE_MCACTIVESYNCMESSAGE_H

#define MAILCORE_MCACTIVESYNCMESSAGE_H

#include <MailCore/MCAbstractMessage.h>
#include <MailCore/MCActiveSyncBody.h>

#ifdef __cplusplus

namespace mailcore {

    class MAILCORE_EXPORT ActiveSyncMessage : public AbstractMessage {
    public:
        ActiveSyncMessage();
        virtual ~ActiveSyncMessage();

        virtual void setServerID(String * serverID);
        virtual String * serverID();
        virtual void setMessageClass(String * messageClass);
        virtual String * messageClass();
        virtual void setEstimatedSize(uint32_t estimatedSize);
        virtual uint32_t estimatedSize();
        virtual void setRead(bool read);
        virtual bool isRead();
        virtual void setFlagged(bool flagged);
        virtual bool isFlagged();
        virtual void setMIMEData(Data * MIMEData);
        virtual Data * MIMEData();
        virtual void setBody(ActiveSyncBody * body);
        virtual ActiveSyncBody * body();
        virtual Array * /* AbstractPart */ attachments();
        virtual Array * /* AbstractPart */ htmlInlineAttachments();
        virtual AbstractPart * partForContentID(String * contentID);
        virtual AbstractPart * partForUniqueID(String * uniqueID);

        virtual Object * copy();
        virtual String * description();

    private:
        String * mServerID;
        String * mMessageClass;
        uint32_t mEstimatedSize;
        bool mRead;
        bool mFlagged;
        Data * mMIMEData;
        ActiveSyncBody * mBody;
        void init();
    };

}

#endif

#endif
