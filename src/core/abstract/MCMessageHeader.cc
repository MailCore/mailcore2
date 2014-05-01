#include "MCMessageHeader.h"

#include "MCAddress.h"
#include "MCIterator.h"

#include <string.h>
#include <unistd.h>
#include <libetpan/libetpan.h>

using namespace mailcore;

static time_t timestamp_from_date(struct mailimf_date_time * date_time);
static struct mailimf_date_time * get_date_from_timestamp(time_t timeval);
static time_t timestamp_from_imap_date(struct mailimap_date_time * date_time);
static time_t mkgmtime(struct tm * tmp);
static int tmcomp(struct tm * atmp, struct tm * btmp);

static struct mailimf_address_list * lep_address_list_from_array(Array * addresses);
static struct mailimf_mailbox_list * lep_mailbox_list_from_array(Array * addresses);
static Array * lep_address_list_from_lep_addr(struct mailimf_address_list * addr_list);
static Array * lep_address_list_from_lep_mailbox(struct mailimf_mailbox_list * mb_list);

static Array * msg_id_to_string_array(clist * msgids);
static clist * msg_id_from_string_array(Array * msgids);

#define MAX_HOSTNAME 512

MessageHeader::MessageHeader()
{
    init(true, true);
}

MessageHeader::MessageHeader(MessageHeader * other)
{
    init(false, other->mMessageID == NULL);
    setMessageID(other->mMessageID);
    setReferences(other->mReferences);
    setInReplyTo(other->mInReplyTo);
    setSender(other->mSender);
    setFrom(other->mFrom);
    setTo(other->mTo);
    setCc(other->mCc);
    setBcc(other->mBcc);
    setReplyTo(other->mReplyTo);
    setSubject(other->mSubject);
    setDate(other->date());
    setReceivedDate(other->receivedDate());
    setExtraHeaders(other->mExtraHeaders);
}

void MessageHeader::init(bool generateDate, bool generateMessageID)
{
    mMessageID = NULL;
    mReferences = NULL;
    mInReplyTo = NULL;
    mSender = NULL;
    mFrom = NULL;
    mTo = NULL;
    mCc = NULL;
    mBcc = NULL;
    mReplyTo = NULL;
    mSubject = NULL;
    mDate = (time_t) -1;
    mReceivedDate = (time_t) -1;
    mExtraHeaders = NULL;
    
    if (generateDate) {
        time_t date;
        date = time(NULL);
        setDate(date);
        setReceivedDate(date);
    }
    if (generateMessageID) {
        static String * hostname = NULL;
        static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
        
        pthread_mutex_lock(&lock);
        if (hostname == NULL) {
            char name[MAX_HOSTNAME];
            int r;

            r = gethostname(name, MAX_HOSTNAME);
            if (r < 0) {
                hostname = NULL;
            }
            else {
                hostname = new String(name);
            }
            if (hostname == NULL) {
                hostname = new String("localhost");
            }
        }
        pthread_mutex_unlock(&lock);
        
        String * messageID = new String();
        messageID->appendString(String::uuidString());
        messageID->appendUTF8Characters("@");
        messageID->appendString(hostname);
        setMessageID(messageID);
        messageID->release();
    }
}

MessageHeader::~MessageHeader()
{
    MC_SAFE_RELEASE(mMessageID);
    MC_SAFE_RELEASE(mReferences);
    MC_SAFE_RELEASE(mInReplyTo);
    MC_SAFE_RELEASE(mSender);
    MC_SAFE_RELEASE(mFrom);
    MC_SAFE_RELEASE(mTo);
    MC_SAFE_RELEASE(mCc);
    MC_SAFE_RELEASE(mBcc);
    MC_SAFE_RELEASE(mReplyTo);
    MC_SAFE_RELEASE(mSubject);
    MC_SAFE_RELEASE(mExtraHeaders);
}

String * MessageHeader::description()
{
    String * result = String::string();
    result->appendUTF8Format("<%s:%p\n", className()->UTF8Characters(), this);
    if (mMessageID != NULL) {
        result->appendUTF8Format("Message-ID: %s\n", mMessageID->UTF8Characters());
    }
    if (mReferences != NULL) {
        result->appendUTF8Format("References: %s\n", mReferences->description()->UTF8Characters());
    }
    if (mInReplyTo != NULL) {
        result->appendUTF8Format("In-Reply-To: %s\n", mInReplyTo->description()->UTF8Characters());
    }
    if (mSender != NULL) {
        result->appendUTF8Format("Sender: %s\n", mSender->description()->UTF8Characters());
    }
    if (mFrom != NULL) {
        result->appendUTF8Format("From: %s\n", mFrom->description()->UTF8Characters());
    }
    if (mTo != NULL) {
        result->appendUTF8Format("To: %s\n", mTo->description()->UTF8Characters());
    }
    if (mCc != NULL) {
        result->appendUTF8Format("Cc: %s\n", mCc->description()->UTF8Characters());
    }
    if (mBcc != NULL) {
        result->appendUTF8Format("Bcc: %s\n", mBcc->description()->UTF8Characters());
    }
    if (mReplyTo != NULL) {
        result->appendUTF8Format("Reply-To: %s\n", mReplyTo->description()->UTF8Characters());
    }
    if (mSubject != NULL) {
        result->appendUTF8Format("Subject: %s\n", mSubject->UTF8Characters());
    }
    if (mExtraHeaders != NULL) {
        mc_foreachhashmapKeyAndValue(String, header, String, value, mExtraHeaders) {
            result->appendUTF8Format("%s: %s\n", header->UTF8Characters(), value->UTF8Characters());
        }
    }
    result->appendUTF8Format(">");
    
    return result;
}

Object * MessageHeader::copy()
{
    return new MessageHeader(this);
}

void MessageHeader::setMessageID(String * messageID)
{
    MC_SAFE_REPLACE_COPY(String, mMessageID, messageID);
}

String * MessageHeader::messageID()
{
    return mMessageID;
}

void MessageHeader::setReferences(Array * references)
{
    MC_SAFE_REPLACE_COPY(Array, mReferences, references);
}

Array * MessageHeader::references()
{
    return mReferences;
}

void MessageHeader::setInReplyTo(Array * inReplyTo)
{
    MC_SAFE_REPLACE_COPY(Array, mInReplyTo, inReplyTo);
}

Array * MessageHeader::inReplyTo()
{
    return mInReplyTo;
}

void MessageHeader::setDate(time_t date)
{
    mDate = date;
}

time_t MessageHeader::date()
{
    return mDate;
}

void MessageHeader::setReceivedDate(time_t date)
{
    mReceivedDate = date;
}

time_t MessageHeader::receivedDate()
{
    return mReceivedDate;
}

void MessageHeader::setSender(Address * sender)
{
    MC_SAFE_REPLACE_RETAIN(Address, mSender, sender);
}

Address * MessageHeader::sender()
{
    return mSender;
}

void MessageHeader::setFrom(Address * from)
{
    MC_SAFE_REPLACE_RETAIN(Address, mFrom, from);
}

Address * MessageHeader::from()
{
    return mFrom;
}

void MessageHeader::setTo(Array * to)
{
    MC_SAFE_REPLACE_COPY(Array, mTo, to);
}

Array * MessageHeader::to()
{
    return mTo;
}

void MessageHeader::setCc(Array * cc)
{
    MC_SAFE_REPLACE_COPY(Array, mCc, cc);
}

Array * MessageHeader::cc()
{
    return mCc;
}

void MessageHeader::setBcc(Array * bcc)
{
    MC_SAFE_REPLACE_COPY(Array, mBcc, bcc);
}

Array * MessageHeader::bcc()
{
    return mBcc;
}

void MessageHeader::setReplyTo(Array * replyTo)
{
    MC_SAFE_REPLACE_COPY(Array, mReplyTo, replyTo);
}

Array * MessageHeader::replyTo()
{
    return mReplyTo;
}

void MessageHeader::setSubject(String * subject)
{
    MC_SAFE_REPLACE_COPY(String, mSubject, subject);
}

String * MessageHeader::subject()
{
    return mSubject;
}

void MessageHeader::setUserAgent(String * userAgent)
{
    setExtraHeader(MCSTR("X-Mailer"), userAgent);
}

String * MessageHeader::userAgent()
{
    return extraHeaderValueForName(MCSTR("X-Mailer"));
}

void MessageHeader::setExtraHeaders(HashMap * headers)
{
    MC_SAFE_REPLACE_COPY(HashMap, mExtraHeaders, headers);
}

Array * MessageHeader::allExtraHeadersNames()
{
    if (mExtraHeaders == NULL)
        return Array::array();
    return mExtraHeaders->allKeys();
}

void MessageHeader::setExtraHeader(String * name, String * object)
{
    if (mExtraHeaders == NULL)
        mExtraHeaders = new HashMap();
    if (object == NULL) {
        removeExtraHeader(name);
        return;
    }
    mExtraHeaders->setObjectForKey(name, object);
}

void MessageHeader::removeExtraHeader(String * name)
{
    if (mExtraHeaders == NULL)
        return;
    mExtraHeaders->removeObjectForKey(name);
}

String * MessageHeader::extraHeaderValueForName(String * name)
{
    if (mExtraHeaders == NULL)
        return NULL;
    return (String *) mExtraHeaders->objectForKey(name);
}

String * MessageHeader::extractedSubject()
{
    if (subject() == NULL)
        return NULL;
    return subject()->extractedSubject();
}

String * MessageHeader::partialExtractedSubject()
{
    if (subject() == NULL)
        return NULL;
    return subject()->extractedSubjectAndKeepBracket(true);
}

void MessageHeader::importHeadersData(Data * data)
{
    size_t cur_token;
    struct mailimf_fields * fields;
    int r;
    
    cur_token = 0;
    r = mailimf_envelope_and_optional_fields_parse(data->bytes(), data->length(), &cur_token, &fields);
    if (r != MAILIMF_NO_ERROR) {
        return;
    }
    
    importIMFFields(fields);

    mailimf_fields_free(fields);
}

void MessageHeader::importIMFFields(struct mailimf_fields * fields)
{
    struct mailimf_single_fields single_fields;
    
    mailimf_single_fields_init(&single_fields, fields);
    
    /* date */
    
    if (single_fields.fld_orig_date != NULL) {
        time_t timestamp;
        timestamp = timestamp_from_date(single_fields.fld_orig_date->dt_date_time);
        setDate(timestamp);
        setReceivedDate(timestamp);
        //MCLog("%lu %lu", (unsigned long) timestamp, date());
    }
    
    /* subject */
    if (single_fields.fld_subject != NULL) {
        char * subject;
        
        subject = single_fields.fld_subject->sbj_value;
        setSubject(String::stringByDecodingMIMEHeaderValue(subject));
    }
    
    /* sender */
    if (single_fields.fld_sender != NULL) {
        struct mailimf_mailbox * mb;
        Address * address;
        
        mb = single_fields.fld_sender->snd_mb;
        if (mb != NULL) {
            address = Address::addressWithIMFMailbox(mb);
            setSender(address);
        }
    }
    
    /* from */
    if (single_fields.fld_from != NULL) {
        struct mailimf_mailbox_list * mb_list;
        Array * addresses;
        
        mb_list = single_fields.fld_from->frm_mb_list;
        addresses = lep_address_list_from_lep_mailbox(mb_list);
        if (addresses->count() > 0) {
            setFrom((Address *) (addresses->objectAtIndex(0)));
        }
    }
    
    /* replyto */
    if (single_fields.fld_reply_to != NULL) {
        struct mailimf_address_list * addr_list;
        Array * addresses;
        
        addr_list = single_fields.fld_reply_to->rt_addr_list;
        addresses = lep_address_list_from_lep_addr(addr_list);
        setReplyTo(addresses);
    }
    
    /* to */
    if (single_fields.fld_to != NULL) {
        struct mailimf_address_list * addr_list;
        Array * addresses;
        
        addr_list = single_fields.fld_to->to_addr_list;
        addresses = lep_address_list_from_lep_addr(addr_list);
        setTo(addresses);
    }
    
    /* cc */
    if (single_fields.fld_cc != NULL) {
        struct mailimf_address_list * addr_list;
        Array * addresses;
        
        addr_list = single_fields.fld_cc->cc_addr_list;
        addresses = lep_address_list_from_lep_addr(addr_list);
        setCc(addresses);
    }
    
    /* bcc */
    if (single_fields.fld_bcc != NULL) {
        struct mailimf_address_list * addr_list;
        Array * addresses;
        
        addr_list = single_fields.fld_bcc->bcc_addr_list;
        addresses = lep_address_list_from_lep_addr(addr_list);
        setBcc(addresses);
    }
    
    /* msgid */
    if (single_fields.fld_message_id != NULL) {
        char * msgid;
        String * str;
        
        msgid = single_fields.fld_message_id->mid_value;
        str = String::stringWithUTF8Characters(msgid);
        setMessageID(str);
    }
    
    /* references */
    if (single_fields.fld_references != NULL) {
        clist * msg_id_list;
        Array * msgids;
        
        msg_id_list = single_fields.fld_references->mid_list;
        msgids = msg_id_to_string_array(msg_id_list);
        setReferences(msgids);
    }
    
    /* inreplyto */
    if (single_fields.fld_in_reply_to != NULL) {
        clist * msg_id_list;
        Array * msgids;
        
        msg_id_list = single_fields.fld_in_reply_to->mid_list;
        msgids = msg_id_to_string_array(msg_id_list);
        setInReplyTo(msgids);
    }
    
    // Take care of other headers.
    for(clistiter * cur = clist_begin(fields->fld_list) ; cur != NULL ; cur = clist_next(cur)) {
        struct mailimf_field * field;

        field = (mailimf_field *)clist_content(cur);

        if (field->fld_type != MAILIMF_FIELD_OPTIONAL_FIELD) {
            continue;
        }
        
        char * fieldName;
        String * fieldNameStr;
        
        fieldName = field->fld_data.fld_optional_field->fld_name;
        fieldNameStr = String::stringWithUTF8Characters(fieldName);
        // Set only if this optional-field is not set
        if (extraHeaderValueForName(fieldNameStr) == NULL) {
            char * fieldValue;
            String * fieldValueStr;
            
            fieldValue = field->fld_data.fld_optional_field->fld_value;
            fieldValueStr = String::stringWithUTF8Characters(fieldValue);
            setExtraHeader(fieldNameStr, fieldValueStr);
        }
    }
}

static time_t timestamp_from_date(struct mailimf_date_time * date_time)
{
    struct tm tmval;
    time_t timeval;
    int zone_min;
    int zone_hour;
    
    tmval.tm_sec  = date_time->dt_sec;
    tmval.tm_min  = date_time->dt_min;
    tmval.tm_hour = date_time->dt_hour;
    tmval.tm_mday = date_time->dt_day;
    tmval.tm_mon  = date_time->dt_month - 1;
    if (date_time->dt_year < 1000) {
        // workaround when century is not given in year
        tmval.tm_year = date_time->dt_year + 2000 - 1900;
    }
    else {
        tmval.tm_year = date_time->dt_year - 1900;
    }
    
    timeval = mkgmtime(&tmval);
    
    if (date_time->dt_zone >= 0) {
        zone_hour = date_time->dt_zone / 100;
        zone_min = date_time->dt_zone % 100;
    }
    else {
        zone_hour = -((- date_time->dt_zone) / 100);
        zone_min = -((- date_time->dt_zone) % 100);
    }
    timeval -= zone_hour * 3600 + zone_min * 60;
    
    return timeval;
}

static struct mailimf_date_time * get_date_from_timestamp(time_t timeval)
{
    struct tm gmt;
    struct tm lt;
    int off;
    struct mailimf_date_time * date_time;
    int sign;
    int hour;
    int min;
    
    gmtime_r(&timeval, &gmt);
    localtime_r(&timeval, &lt);
    
    off = (int) ((mkgmtime(&lt) - mkgmtime(&gmt)) / 60);
    if (off < 0) {
        sign = -1;
    }
    else {
        sign = 1;
    }
    off = off * sign;
    min = off % 60;
    hour = off / 60;
    off = hour * 100 + min;
    off = off * sign;
    
    date_time = mailimf_date_time_new(lt.tm_mday, lt.tm_mon + 1,
                                      lt.tm_year + 1900,
                                      lt.tm_hour, lt.tm_min, lt.tm_sec,
                                      off);
    
    return date_time;
}

static time_t timestamp_from_imap_date(struct mailimap_date_time * date_time)
{
    struct tm tmval;
    time_t timeval;
    int zone_min;
    int zone_hour;
    
    tmval.tm_sec  = date_time->dt_sec;
    tmval.tm_min  = date_time->dt_min;
    tmval.tm_hour = date_time->dt_hour;
    tmval.tm_mday = date_time->dt_day;
    tmval.tm_mon  = date_time->dt_month - 1;
    if (date_time->dt_year < 1000) {
        // workaround when century is not given in year
        tmval.tm_year = date_time->dt_year + 2000 - 1900;
    }
    else {
        tmval.tm_year = date_time->dt_year - 1900;
    }
    
    timeval = mkgmtime(&tmval);
    
    if (date_time->dt_zone >= 0) {
        zone_hour = date_time->dt_zone / 100;
        zone_min = date_time->dt_zone % 100;
    }
    else {
        zone_hour = -((- date_time->dt_zone) / 100);
        zone_min = -((- date_time->dt_zone) % 100);
    }
    timeval -= zone_hour * 3600 + zone_min * 60;
    
    return timeval;
}

#define INVALID_TIMESTAMP    (-1)

static int tmcomp(struct tm * atmp, struct tm * btmp)
{
    int    result;
    
    if ((result = (atmp->tm_year - btmp->tm_year)) == 0 &&
        (result = (atmp->tm_mon - btmp->tm_mon)) == 0 &&
        (result = (atmp->tm_mday - btmp->tm_mday)) == 0 &&
        (result = (atmp->tm_hour - btmp->tm_hour)) == 0 &&
        (result = (atmp->tm_min - btmp->tm_min)) == 0)
        result = atmp->tm_sec - btmp->tm_sec;
    return result;
}

static time_t mkgmtime(struct tm * tmp)
{
    int            dir;
    int            bits;
    int            saved_seconds;
    time_t                t;
    struct tm            yourtm, mytm;
    
    yourtm = *tmp;
    saved_seconds = yourtm.tm_sec;
    yourtm.tm_sec = 0;
    /*
     ** Calculate the number of magnitude bits in a time_t
     ** (this works regardless of whether time_t is
     ** signed or unsigned, though lint complains if unsigned).
     */
    for (bits = 0, t = 1; t > 0; ++bits, t <<= 1)
        ;
    /*
     ** If time_t is signed, then 0 is the median value,
     ** if time_t is unsigned, then 1 << bits is median.
     */
    if(bits > 40) bits = 40;
    t = (t < 0) ? 0 : ((time_t) 1 << bits);
    for ( ; ; ) {
        gmtime_r(&t, &mytm);
        dir = tmcomp(&mytm, &yourtm);
        if (dir != 0) {
            if (bits-- < 0) {
                return INVALID_TIMESTAMP;
            }
            if (bits < 0)
                --t;
            else if (dir > 0)
                t -= (time_t) 1 << bits;
            else    t += (time_t) 1 << bits;
            continue;
        }
        break;
    }
    t += saved_seconds;
    return t;
}

#pragma mark RFC 2822 mailbox conversion

static Array * lep_address_list_from_lep_mailbox(struct mailimf_mailbox_list * mb_list)
{
    Array * result;
    clistiter * cur;
    
    result = Array::array();
    for(cur = clist_begin(mb_list->mb_list) ; cur != NULL ; cur = clist_next(cur)) {
        struct mailimf_mailbox * mb;
        Address * address;
        
        mb = (struct mailimf_mailbox *) clist_content(cur);
        address = Address::addressWithIMFMailbox(mb);
        result->addObject(address);
    }
    
    return result;
}

static Array * lep_address_list_from_lep_addr(struct mailimf_address_list * addr_list)
{
    Array * result;
    clistiter * cur;
    
    result = Array::array();
    
    if (addr_list == NULL) {
        return result;
    }
    
    if (addr_list->ad_list == NULL) {
        return result;
    }
    
    for(cur = clist_begin(addr_list->ad_list) ; cur != NULL ;
        cur = clist_next(cur)) {
        struct mailimf_address * addr;
        
        addr = (struct mailimf_address *) clist_content(cur);
        switch (addr->ad_type) {
            case MAILIMF_ADDRESS_MAILBOX:
            {
                Address * address;
                
                address = Address::addressWithIMFMailbox(addr->ad_data.ad_mailbox);
                result->addObject(address);
                break;
            }
            
            case MAILIMF_ADDRESS_GROUP:
            {
                if (addr->ad_data.ad_group->grp_mb_list != NULL) {
                    Array * subArray;
                    
                    subArray = lep_address_list_from_lep_mailbox(addr->ad_data.ad_group->grp_mb_list);
                    result->addObjectsFromArray(subArray);
                }
                break;
            }
        }
    }
    
    return result;
}

static struct mailimf_mailbox_list * lep_mailbox_list_from_array(Array * addresses)
{
    struct mailimf_mailbox_list * mb_list;
    
    if (addresses == NULL)
        return NULL;
    
    if (addresses->count() == 0)
        return NULL;
    
    mb_list = mailimf_mailbox_list_new_empty();
    
    for(unsigned i = 0 ; i < addresses->count() ; i ++) {
        Address * address = (Address *) addresses->objectAtIndex(i);
        struct mailimf_mailbox * mailbox = address->createIMFMailbox();
        mailimf_mailbox_list_add(mb_list, mailbox);
    }
    
    return mb_list;
}

static struct mailimf_address_list * lep_address_list_from_array(Array * addresses)
{
    struct mailimf_address_list * addr_list;
    
    if (addresses == NULL)
        return NULL;
    
    if (addresses->count() == 0)
        return NULL;
    
    addr_list = mailimf_address_list_new_empty();

    for(unsigned i = 0 ; i < addresses->count() ; i ++) {
        Address * address = (Address *) addresses->objectAtIndex(i);
        struct mailimf_address * addr = address->createIMFAddress();
        mailimf_address_list_add(addr_list, addr);
    }
    
    return addr_list;
}

#pragma mark Message-ID conversion

static Array * msg_id_to_string_array(clist * msgids)
{
    clistiter * cur;
    Array * result;
    
    result = Array::array();
    
    for(cur = clist_begin(msgids) ; cur != NULL ; cur = clist_next(cur)) {
        char * msgid;
        String * str;
        
        msgid = (char *) clist_content(cur);
        str = String::stringWithUTF8Characters(msgid);
        result->addObject(str);
    }
    
    return result;
}

static clist * msg_id_from_string_array(Array * msgids)
{
    clist * result;
    
    if (msgids == NULL)
        return NULL;
    
    if (msgids->count() == 0)
        return NULL;
    
    result = clist_new();
    for(unsigned int i = 0 ; i < msgids->count() ; i ++) {
        String * msgid = (String *) msgids->objectAtIndex(i);
        clist_append(result, strdup(msgid->UTF8Characters()));
    }
    
    return result;
}

struct mailimf_fields * MessageHeader::createIMFFieldsAndFilterBcc(bool filterBcc)
{
    struct mailimf_date_time * imfDate;
    char * imfMsgid;
    char * imfSubject;
    struct mailimf_mailbox_list * imfFrom;
    struct mailimf_address_list * imfReplyTo;
    struct mailimf_address_list * imfTo;
    struct mailimf_address_list * imfCc;
    struct mailimf_address_list * imfBcc;
    clist * imfInReplyTo;
    clist * imfReferences;
    struct mailimf_fields * fields;
    
    imfDate = NULL;
    if (date() != (time_t) -1) {
        //MCLog("%lu", date());
        imfDate = get_date_from_timestamp(date());
    }
    imfFrom = NULL;
    if (from() != NULL) {
        imfFrom = lep_mailbox_list_from_array(Array::arrayWithObject(from()));
    }
    imfReplyTo = lep_address_list_from_array(replyTo());
    imfTo = lep_address_list_from_array(to());
    imfCc = lep_address_list_from_array(cc());
    imfBcc = NULL;
    if (!filterBcc) {
        imfBcc = lep_address_list_from_array(bcc());
    }
    imfMsgid = NULL;
    if (messageID() != NULL) {
        imfMsgid = strdup(messageID()->UTF8Characters());
    }
    imfInReplyTo = msg_id_from_string_array(inReplyTo());
    imfReferences = msg_id_from_string_array(references());
    imfSubject = NULL;
    if ((subject() != NULL) && (subject()->length() > 0)) {
        Data * data;
        
        data = subject()->encodedMIMEHeaderValueForSubject();
        if (data->bytes() != NULL) {
            imfSubject = strdup(data->bytes());
        }
    }
    
    if ((imfTo == NULL) && (imfCc == NULL) && (imfBcc == NULL)) {
        imfTo = mailimf_address_list_new_empty();
        mailimf_address_list_add_parse(imfTo, (char *) "Undisclosed recipients:;");
    }
    
    fields = mailimf_fields_new_with_data_all(imfDate,
        imfFrom,
        NULL /* sender */,
        imfReplyTo,
        imfTo,
        imfCc,
        imfBcc,
        imfMsgid,
        imfInReplyTo,
        imfReferences,
        imfSubject);
    
    if (mExtraHeaders != NULL) {
        mc_foreachhashmapKeyAndValue(String, header, String, value, mExtraHeaders) {
            struct mailimf_field * field;
            
            field = mailimf_field_new_custom(strdup(header->UTF8Characters()), strdup(value->UTF8Characters()));
            mailimf_fields_add(fields, field);
        }
    }
    
    return fields;
}

extern "C" {
    extern int mailimap_hack_date_time_parse(char * str,
        struct mailimap_date_time ** result,
        size_t progr_rate,
        progress_function * progr_fun);
}

#pragma mark IMAP mailbox conversion

static Array * imap_mailbox_list_to_address_array(clist * imap_mailbox_list)
{
    clistiter * cur;
    Array * result;
    
    result = Array::array();
    
    for(cur = clist_begin(imap_mailbox_list) ; cur != NULL ;
        cur = clist_next(cur)) {
        struct mailimap_address * imap_addr;
        Address * address;
        
        imap_addr = (struct mailimap_address *) clist_content(cur);
        address = Address::addressWithIMAPAddress(imap_addr);
        result->addObject(address);
    }
    
    return result;
}

void MessageHeader::importIMAPEnvelope(struct mailimap_envelope * env)
{
    if (env->env_date != NULL) {
        size_t cur_token;
        struct mailimf_date_time * date_time;
        int r;

        cur_token = 0;
        r = mailimf_date_time_parse(env->env_date, strlen(env->env_date),
            &cur_token, &date_time);
        if (r == MAILIMF_NO_ERROR) {
            time_t timestamp;

            // date
            timestamp = timestamp_from_date(date_time);
            setDate(timestamp);
            setReceivedDate(timestamp);
            mailimf_date_time_free(date_time);
        }
        else {
            struct mailimap_date_time * imap_date;

            r = mailimap_hack_date_time_parse(env->env_date, &imap_date, 0, NULL);
            if (r == MAILIMAP_NO_ERROR) {
                time_t timestamp;

                timestamp = timestamp_from_imap_date(imap_date);
                setDate(timestamp);
                setReceivedDate(timestamp);
                mailimap_date_time_free(imap_date);
            }
        }
    }

    if (env->env_subject != NULL) {
        char * subject;

        // subject
        subject = env->env_subject;
        setSubject(String::stringByDecodingMIMEHeaderValue(subject));
    }

    if (env->env_sender != NULL) {
        if (env->env_sender->snd_list != NULL) {
            Array * addresses;

            addresses = imap_mailbox_list_to_address_array(env->env_sender->snd_list);
            if (addresses->count() > 0) {
                setSender((Address *) addresses->objectAtIndex(0));
            }
        }
    }

    if (env->env_from != NULL) {
        if (env->env_from->frm_list != NULL) {
            Array * addresses;

            addresses = imap_mailbox_list_to_address_array(env->env_from->frm_list);
            if (addresses->count() > 0) {
                setFrom((Address *) addresses->objectAtIndex(0));
            }
        }
    }

        // skip Sender header

    if (env->env_reply_to != NULL) {
        if (env->env_reply_to->rt_list != NULL) {
            Array * addresses;

            addresses = imap_mailbox_list_to_address_array(env->env_reply_to->rt_list);
            setReplyTo(addresses);
        }
    }

    if (env->env_to != NULL) {
        if (env->env_to->to_list != NULL) {
            Array * addresses;

            addresses = imap_mailbox_list_to_address_array(env->env_to->to_list);
            setTo(addresses);
        }
    }

    if (env->env_cc != NULL) {
        if (env->env_cc->cc_list != NULL) {
            Array * addresses;

            addresses = imap_mailbox_list_to_address_array(env->env_cc->cc_list);
            setCc(addresses);
        }
    }

    if (env->env_bcc != NULL) {
        if (env->env_bcc->bcc_list != NULL) {
            Array * addresses;

            addresses = imap_mailbox_list_to_address_array(env->env_bcc->bcc_list);
            setBcc(addresses);
        }
    }

    if (env->env_in_reply_to != NULL) {
        size_t cur_token;
        clist * msg_id_list;
        int r;

        cur_token = 0;
        r = mailimf_msg_id_list_parse(env->env_in_reply_to,
            strlen(env->env_in_reply_to), &cur_token, &msg_id_list);
        if (r == MAILIMF_NO_ERROR) {
            Array * msgids;

            msgids = msg_id_to_string_array(msg_id_list);
            setInReplyTo(msgids);
            // in-reply-to
            clist_foreach(msg_id_list, (clist_func) mailimf_msg_id_free, NULL);
            clist_free(msg_id_list);
        }
    }

    if (env->env_message_id != NULL) {
        char * msgid;
        size_t cur_token;
        int r;

        cur_token = 0;
        r = mailimf_msg_id_parse(env->env_message_id, strlen(env->env_message_id),
            &cur_token, &msgid);
        if (r == MAILIMF_NO_ERROR) {
            // msg id
            String * str;

            str = String::stringWithUTF8Characters(msgid);
            setMessageID(str);
            mailimf_msg_id_free(msgid);
        }
    }
}

void MessageHeader::importIMAPReferences(Data * data)
{
    size_t cur_token;
    struct mailimf_fields * fields;
    int r;
    struct mailimf_single_fields single_fields;
    
    cur_token = 0;
    r = mailimf_fields_parse(data->bytes(), data->length(), &cur_token, &fields);
    if (r != MAILIMF_NO_ERROR) {
        return;
    }
    
    mailimf_single_fields_init(&single_fields, fields);
    if (single_fields.fld_references != NULL) {
        Array * msgids;
        
        msgids = msg_id_to_string_array(single_fields.fld_references->mid_list);
        setReferences(msgids);
    }
    if (single_fields.fld_subject != NULL) {
        if (single_fields.fld_subject->sbj_value != NULL) {
            bool broken;
            char * value;
            bool isASCII;
            
            broken = false;
            value = single_fields.fld_subject->sbj_value;
            
            isASCII = true;
            for(char * p = value ; * p != 0 ; p ++) {
                if ((unsigned char) * p >= 128) {
                    isASCII = false;
                }
            }
            if (strstr(value, "windows-1251") == NULL) {
                if (isASCII) {
                    broken = true;
                }
            }
            
            //MCLog("charset: %s %s", value, MCUTF8(charset));
            
            if (!broken) {
                setSubject(String::stringByDecodingMIMEHeaderValue(single_fields.fld_subject->sbj_value));
            }
        }
    }
    
    mailimf_fields_free(fields);
}

void MessageHeader::importIMAPInternalDate(struct mailimap_date_time * date)
{
    setReceivedDate(timestamp_from_imap_date(date));
}

Array * MessageHeader::recipientWithReplyAll(bool replyAll, bool includeTo, bool includeCc, Array * senderEmails)
{
    bool hasCc;
    bool hasTo;
    Set * addedAddresses;
    Array * toField;
    Array * ccField;
    bool containsSender;
    
    toField = NULL;
    ccField = NULL;
    
    hasTo = false;
    hasCc = false;
    addedAddresses = new Set();
    
    containsSender = false;
    if (senderEmails != NULL) {
      if (from() != NULL) {
        if (senderEmails->containsObject(from()->mailbox()->lowercaseString())) {
          containsSender = true;
        }
      }
      if (sender() != NULL) {
        if (senderEmails->containsObject(sender()->mailbox()->lowercaseString())) {
          containsSender = true;
        }
      }
    }
    
    if (containsSender) {
        Array * recipient;
        
        recipient = new Array();
        if (to() != NULL) {
            for(unsigned int i = 0 ; i < to()->count() ; i ++) {
                Address * address = (Address *) to()->objectAtIndex(i);
                if (addedAddresses->containsObject(address->mailbox()->lowercaseString())) {
                    continue;
                }
                if ((from() != NULL) && address->mailbox()->isEqualCaseInsensitive(from()->mailbox())) {
                    recipient->addObjectsFromArray(replyTo());
                    for(unsigned int j = 0 ; j < replyTo()->count() ; j ++) {
                        Address * rtAddress = (Address *) replyTo()->objectAtIndex(j);
                        if (addedAddresses->containsObject(rtAddress->mailbox()->lowercaseString())) {
                            continue;
                        }
                        addedAddresses->addObject(rtAddress->mailbox()->lowercaseString());
                    }
                }
                else {
                    if (address->mailbox() != NULL) {
                        recipient->addObject(address);
                        addedAddresses->addObject(address->mailbox()->lowercaseString());
                    }
                }
                hasTo = true;
            }
        }
        toField = recipient;
        toField->retain()->autorelease();
        recipient->release();
        
        if (replyAll) {
            recipient = new Array();
             if (cc() != NULL) {
               for(unsigned int i = 0 ; i < cc()->count() ; i ++) {
                    Address * address = (Address *) cc()->objectAtIndex(i);
                    if (addedAddresses->containsObject(address->mailbox()->lowercaseString())) {
                        continue;
                    }
                    if (address->mailbox() == NULL)
                        continue;
                    recipient->addObject(address);
                    addedAddresses->addObject(address->mailbox()->lowercaseString());
                    hasCc = true;
                }
            }
            ccField = recipient;
            ccField->retain()->autorelease();
            recipient->release();
        }
        
        if (!hasTo && !hasCc) {
            hasTo = true;
            toField = Array::arrayWithObject(from());
        }
    }
    else {
        addedAddresses->addObjectsFromArray(senderEmails);
        
        if (replyTo() != NULL && replyTo()->count() > 0) {
            hasTo = true;
            toField = replyTo();
            for(unsigned int i = 0 ; i < replyTo()->count() ; i ++) {
                Address * address = (Address *) replyTo()->objectAtIndex(i);
                if (address->mailbox() == NULL)
                    continue;
                addedAddresses->addObject(address->mailbox()->lowercaseString());
            }
        }
        else {
            if (from() != NULL && from()->mailbox() != NULL) {
                hasTo = true;
                toField = Array::arrayWithObject(from());
                addedAddresses->addObject(from()->mailbox()->lowercaseString());
            }
        }
        
        if (replyAll) {
            Array * recipient;
            
            recipient = new Array();
            if (to() != NULL) {
                for(unsigned int i = 0 ; i < to()->count() ; i ++) {
                    Address * address = (Address *) to()->objectAtIndex(i);
                    if (addedAddresses->containsObject(address->mailbox()->lowercaseString())) {
                        continue;
                    }
                    if (address->mailbox() == NULL)
                        continue;
                    recipient->addObject(address);
                    addedAddresses->addObject(address->mailbox()->lowercaseString());
                }
            }
            if (cc() != NULL) {
                for(unsigned int i = 0 ; i < cc()->count() ; i ++) {
                    Address * address = (Address *) cc()->objectAtIndex(i);
                    if (addedAddresses->containsObject(address->mailbox()->lowercaseString())) {
                        continue;
                    }
                    if (address->mailbox() == NULL)
                        continue;
                    recipient->addObject(address);
                    addedAddresses->addObject(address->mailbox()->lowercaseString());
                }
            }
            if (recipient->count() > 0) {
                hasCc = true;
            }
            ccField = recipient;
            ccField->retain()->autorelease();
            recipient->release();
        }
    }
    
    addedAddresses->release();
    
    Array * result;
    result = Array::array();
    if (hasTo && includeTo)
        result->addObjectsFromArray(toField);
    if (hasCc && includeCc)
        result->addObjectsFromArray(ccField);
    
    return result;
}

MessageHeader * MessageHeader::replyHeader(bool replyAll, Array * addressesExcludedFromRecipient)
{
    MessageHeader * result;
    String * subjectValue;
    Array * referencesValue;
    Array * inReplyTo;
    Array * toValue;
    Array * ccValue;
    
    referencesValue = NULL;
    inReplyTo = NULL;
    
    result = new MessageHeader();
    if (subject() == NULL) {
        subjectValue = MCSTR("Re: ");
    }
    else {
        subjectValue = MCSTR("Re: ")->stringByAppendingString(subject());
    }
    if (references() != NULL) {
        referencesValue = (Array *) (references()->copy());
        referencesValue->autorelease();
        if (messageID() != NULL ) {
            referencesValue->addObject(messageID());
        }
    }
    if (messageID()) {
        inReplyTo = Array::array();
        inReplyTo->addObject(messageID());
    }
    toValue = recipientWithReplyAll(replyAll, true, false, addressesExcludedFromRecipient);
    ccValue = recipientWithReplyAll(replyAll, false, true, addressesExcludedFromRecipient);;
    
    result->setSubject(subjectValue);
    result->setReferences(referencesValue);
    result->setInReplyTo(inReplyTo);
    result->setTo(toValue);
    result->setCc(ccValue);
    
    result->autorelease();
    return result;
}

MessageHeader * MessageHeader::forwardHeader()
{
    MessageHeader * result;
    String * subjectValue;
    Array * referencesValue;
    Array * inReplyTo;
    
    referencesValue = NULL;
    inReplyTo = NULL;

    result = new MessageHeader();
    if (subject() == NULL) {
        subjectValue = MCSTR("Fw: ");
    }
    else {
        subjectValue = MCSTR("Fw: ")->stringByAppendingString(subject());
    }
    if (references() != NULL) {
        referencesValue = (Array *) (references()->copy());
        referencesValue->autorelease();
        if (messageID() != NULL ) {
            referencesValue->addObject(messageID());
        }
    }
    if (messageID() != NULL) {
        inReplyTo = Array::array();
        inReplyTo->addObject(messageID());
    }
    result->setSubject(subjectValue);
    result->setReferences(referencesValue);
    result->setInReplyTo(inReplyTo);
    
    result->autorelease();
    return result;
}

HashMap * MessageHeader::serializable()
{
    HashMap * result = Object::serializable();
    
    if (messageID() != NULL) {
        result->setObjectForKey(MCSTR("messageID"), messageID());
    }
    if (references() != NULL) {
        result->setObjectForKey(MCSTR("references"), references());
    }
    if (inReplyTo() != NULL) {
        result->setObjectForKey(MCSTR("inReplyTo"), inReplyTo());
    }
    if (sender() != NULL) {
        result->setObjectForKey(MCSTR("sender"), sender()->serializable());
    }
    if (from() != NULL) {
        result->setObjectForKey(MCSTR("from"), from()->serializable());
    }
    if (to() != NULL) {
        result->setObjectForKey(MCSTR("to"), to()->serializable());
    }
    if (cc() != NULL) {
        result->setObjectForKey(MCSTR("cc"), cc()->serializable());
    }
    if (bcc() != NULL) {
        result->setObjectForKey(MCSTR("bcc"), bcc()->serializable());
    }
    if (replyTo() != NULL) {
        result->setObjectForKey(MCSTR("replyTo"), replyTo()->serializable());
    }
    if (subject() != NULL) {
        result->setObjectForKey(MCSTR("subject"), subject());
    }
    result->setObjectForKey(MCSTR("date"), String::stringWithUTF8Format("%lld", (unsigned long long) date()));
    result->setObjectForKey(MCSTR("receivedDate"), String::stringWithUTF8Format("%lld", (unsigned long long) receivedDate()));
    if (mExtraHeaders != NULL) {
        result->setObjectForKey(MCSTR("extraHeaders"), mExtraHeaders);
    }
    
    return result;
}

void MessageHeader::importSerializable(HashMap * hashmap)
{
    setMessageID((String *) hashmap->objectForKey(MCSTR("messageID")));
    setReferences((Array *) hashmap->objectForKey(MCSTR("references")));
    setInReplyTo((Array *) hashmap->objectForKey(MCSTR("inReplyTo")));
    setSender((Address *) Object::objectWithSerializable((HashMap *) hashmap->objectForKey(MCSTR("sender"))));
    setFrom((Address *) Object::objectWithSerializable((HashMap *) hashmap->objectForKey(MCSTR("from"))));
    setTo((Array *) Object::objectWithSerializable((HashMap *) hashmap->objectForKey(MCSTR("to"))));
    setCc((Array *)Object::objectWithSerializable((HashMap *) hashmap->objectForKey(MCSTR("cc"))));
    setBcc((Array *)Object::objectWithSerializable((HashMap *) hashmap->objectForKey(MCSTR("bcc"))));
    setReplyTo((Array *)Object::objectWithSerializable((HashMap *) hashmap->objectForKey(MCSTR("replyTo"))));
    setSubject((String *) hashmap->objectForKey(MCSTR("subject")));
    setDate((time_t) ((String *) hashmap->objectForKey(MCSTR("date")))->unsignedLongLongValue());
    setReceivedDate((time_t) ((String *) hashmap->objectForKey(MCSTR("receivedDate")))->unsignedLongLongValue());
    setExtraHeaders((HashMap *) hashmap->objectForKey(MCSTR("extraHeaders")));
}

static void * createObject()
{
    return new MessageHeader();
}

__attribute__((constructor))
static void initialize()
{
    Object::registerObjectConstructor("mailcore::MessageHeader", &createObject);
}
