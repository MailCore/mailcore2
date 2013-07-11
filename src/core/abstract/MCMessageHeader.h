#ifndef __MAILCORE_MCMESSAGEHEADER_H_

#define __MAILCORE_MCMESSAGEHEADER_H_

#include <MailCore/MCBaseTypes.h>
#include <time.h>

#ifdef __cplusplus

namespace mailcore {

    class Address;

    class MessageHeader : public Object {
    public:
        MessageHeader();
        virtual ~MessageHeader();

        virtual void setMessageID(String * messageID);
        virtual String * messageID();

        virtual void setReferences(Array * /* String */ references);
        virtual Array * /* String */ references();

        virtual void setInReplyTo(Array * /* String */ inReplyTo);
        virtual Array * /* String */ inReplyTo();

        virtual void setDate(time_t date);
        virtual time_t date();

        virtual void setReceivedDate(time_t date);
        virtual time_t receivedDate();

        virtual void setSender(Address * sender);
        virtual Address * sender();

        virtual void setFrom(Address * from);
        virtual Address * from();

        virtual void setTo(Array * /* Address */ to);
        virtual Array * /* Address */ to();

        virtual void setCc(Array * /* Address */ cc);
        virtual Array * /* Address */ cc();

        virtual void setBcc(Array * /* Address */ bcc);
        virtual Array * /* Address */ bcc();

        virtual void setReplyTo(Array * /* Address */ replyTo);
        virtual Array * /* Address */ replyTo();

        virtual void setSubject(String * subject);
        virtual String * subject();

        virtual void setUserAgent(String * userAgent);
        virtual String * userAgent();

        virtual void addHeader(String *name, String * value);
        virtual void removeHeader(String *name);
        virtual String * headerValueForName(String *name);
        virtual Array * allExtraHeadersNames();

        String * extractedSubject();
        virtual String * partialExtractedSubject();
        virtual void importHeadersData(Data * data);

        virtual MessageHeader * replyHeader(bool replyAll, Array * addressesExcludedFromRecipient);
        virtual MessageHeader * forwardHeader();

    public: // subclass behavior
        MessageHeader(MessageHeader * other);
        virtual String * description();
        virtual Object * copy();

    public: // private
        virtual void importIMAPEnvelope(struct mailimap_envelope * env);
        virtual void importIMAPReferences(Data * data);
        virtual void importIMAPInternalDate(struct mailimap_date_time * date);

        virtual struct mailimf_fields * createIMFFieldsAndFilterBcc(bool filterBcc);
        virtual void importIMFFields(struct mailimf_fields * fields);

    private:
        String * mMessageID;
        Array * /* String */ mReferences;
        Array * /* String */ mInReplyTo;
        Address * mSender;
        Address * mFrom;
        Array * /* Address */ mTo;
        Array * /* Address */ mCc;
        Array * /* Address */ mBcc;
        Array * /* Address */ mReplyTo;
        String * mSubject;
        time_t mDate;
        time_t mReceivedDate;
        String * mUserAgent;
        HashMap * mExtraHeaders;
        void init(bool generateDate, bool generateMessageID);
        void setExtraHeaders(HashMap *headers);
        Array * recipientWithReplyAll(bool replyAll, bool includeTo, bool includeCc, Array * senderEmails);
    };

}

#endif

#endif
