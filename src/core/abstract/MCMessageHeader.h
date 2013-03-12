#ifndef __MAILCORE_MCMESSAGEHEADER_H_

#define __MAILCORE_MCMESSAGEHEADER_H_

#include <mailcore/MCBaseTypes.h>
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
		
		virtual void setReferences(Array * references);
		virtual Array * references();
		
		virtual void setInReplyTo(Array * inReplyTo);
		virtual Array * inReplyTo();
		
		virtual void setDate(time_t date);
		virtual time_t date();
		
		virtual void setReceivedDate(time_t date);
		virtual time_t receivedDate();
		
		virtual void setSender(Address * sender);
		virtual Address * sender();
		
		virtual void setFrom(Address * from);
		virtual Address * from();
		
		virtual void setTo(Array * to);
		virtual Array * to();
		
		virtual void setCc(Array * cc);
		virtual Array * cc();
		
		virtual void setBcc(Array * bcc);
		virtual Array * bcc();
		
		virtual void setReplyTo(Array * replyTo);
		virtual Array * replyTo();
		
		virtual void setSubject(String * subject);
		virtual String * subject();
		
		virtual void setUserAgent(String * userAgent);
		virtual String * userAgent();
		
		virtual String * extractedSubject();
		virtual String * partialExtractedSubject();
		virtual void importHeadersData(Data * data);
        
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
		void init(bool generateDate, bool generateMessageID);
	};
	
}

#endif

#endif
