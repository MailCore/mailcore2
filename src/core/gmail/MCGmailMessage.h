#ifndef MAILCORE_MCGMAILMESSAGE_H

#define MAILCORE_MCGMAILMESSAGE_H

#include <MailCore/MCAbstractMessage.h>

#ifdef __cplusplus

namespace mailcore {

    class GmailMessagePart;

    class MAILCORE_EXPORT GmailMessage : public AbstractMessage {
    public:
        GmailMessage();
        virtual ~GmailMessage();

        virtual String * identifier();
        virtual void setIdentifier(String * identifier);

        virtual String * threadID();
        virtual void setThreadID(String * threadID);

        virtual Array * /* String */ labelIDs();
        virtual void setLabelIDs(Array * labelIDs);

        virtual String * snippet();
        virtual void setSnippet(String * snippet);

        virtual String * historyID();
        virtual void setHistoryID(String * historyID);

        virtual String * internalDate();
        virtual void setInternalDate(String * internalDate);

        virtual uint32_t sizeEstimate();
        virtual void setSizeEstimate(uint32_t sizeEstimate);

        virtual Data * RFC822Data();
        virtual void setRFC822Data(Data * RFC822Data);

        virtual GmailMessagePart * payload();
        virtual void setPayload(GmailMessagePart * payload);

        virtual AbstractMessage * parsedMessage(ErrorCode * pError);
        virtual AbstractPart * partForPartID(String * partID);
        virtual AbstractPart * partForContentID(String * contentID);
        virtual AbstractPart * partForUniqueID(String * uniqueID);

    private:
        String * mIdentifier;
        String * mThreadID;
        Array * mLabelIDs;
        String * mSnippet;
        String * mHistoryID;
        String * mInternalDate;
        uint32_t mSizeEstimate;
        Data * mRFC822Data;
        GmailMessagePart * mPayload;

        void init();
    };

}

#endif

#endif
