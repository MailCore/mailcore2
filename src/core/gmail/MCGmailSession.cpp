#include "MCGmailSession.h"

#include <libetpan/mailgmail.h>
#include <string.h>

#include "MCGmailLabel.h"
#include "MCGmailMessage.h"
#include "MCGmailMessageGetRequestPrivate.h"
#include "MCGmailMessageHeader.h"
#include "MCGmailMessageList.h"
#include "MCGmailMessageListRequestPrivate.h"
#include "MCGmailMessagePart.h"
#include "MCGmailMessageSummary.h"
#include "MCGmailProfile.h"

using namespace mailcore;

static ErrorCode errorCodeFromGmailError(int r)
{
    switch (r) {
        case MAILGMAIL_NO_ERROR:
            return ErrorNone;
        case MAILGMAIL_ERROR_UNAUTHORIZED:
        case MAILGMAIL_ERROR_FORBIDDEN:
            return ErrorAuthentication;
        case MAILGMAIL_ERROR_NOT_FOUND:
            return ErrorFetch;
        case MAILGMAIL_ERROR_SSL:
            return ErrorCertificate;
        case MAILGMAIL_ERROR_PARSE:
        case MAILGMAIL_ERROR_PROTOCOL:
            return ErrorParse;
        case MAILGMAIL_ERROR_MEMORY:
        case MAILGMAIL_ERROR_BAD_STATE:
        case MAILGMAIL_ERROR_HTTP:
        case MAILGMAIL_ERROR_HTTP_UNAVAILABLE:
        case MAILGMAIL_ERROR_RATE_LIMITED:
        case MAILGMAIL_ERROR_CONFLICT:
        case MAILGMAIL_ERROR_SERVER:
        case MAILGMAIL_ERROR_NOT_IMPLEMENTED:
        default:
            return ErrorConnection;
    }
}

static enum mailgmail_message_format mailgmailFormatFromGmailFormat(GmailMessageFormat format)
{
    switch (format) {
        case GmailMessageFormatMinimal:
            return MAILGMAIL_MESSAGE_FORMAT_MINIMAL;
        case GmailMessageFormatFull:
            return MAILGMAIL_MESSAGE_FORMAT_FULL;
        case GmailMessageFormatRaw:
            return MAILGMAIL_MESSAGE_FORMAT_RAW;
        case GmailMessageFormatMetadata:
            return MAILGMAIL_MESSAGE_FORMAT_METADATA;
    }

    return MAILGMAIL_MESSAGE_FORMAT_FULL;
}

static String * stringFromNullableCString(const char * value)
{
    if (value == NULL)
        return NULL;

    return String::stringWithUTF8Characters(value);
}

static Data * decodedBase64URLDataFromNullableCString(const char * value, ErrorCode * pError)
{
    if (value == NULL)
        return NULL;

    String * normalized = String::string();
    for (const char * cur = value; * cur != '\0'; cur ++) {
        if (* cur == '-') {
            normalized->appendUTF8Characters("+");
        }
        else if (* cur == '_') {
            normalized->appendUTF8Characters("/");
        }
        else {
            char buffer[2] = { * cur, '\0' };
            normalized->appendUTF8Characters(buffer);
        }
    }

    unsigned int remainder = (unsigned int) strlen(normalized->UTF8Characters()) % 4;
    if (remainder != 0) {
        for (unsigned int i = remainder; i < 4; i ++) {
            normalized->appendUTF8Characters("=");
        }
    }

    Data * result = normalized->decodedBase64Data();
    if (result == NULL) {
        * pError = ErrorParse;
    }
    return result;
}

static Array * stringArrayFromCList(clist * list)
{
    Array * result = Array::array();
    if (list == NULL)
        return result;

    for (clistiter * cur = clist_begin(list); cur != NULL; cur = clist_next(cur)) {
        char * value = (char *) clist_content(cur);
        String * string = stringFromNullableCString(value);
        if (string != NULL) {
            result->addObject(string);
        }
    }

    return result;
}

static GmailProfile * profileFromLibetpan(struct mailgmail_profile * profile)
{
    GmailProfile * result = new GmailProfile();
    result->autorelease();
    result->setEmailAddress(stringFromNullableCString(profile->email_address));
    result->setMessagesTotal(profile->messages_total);
    result->setThreadsTotal(profile->threads_total);
    result->setHistoryID(stringFromNullableCString(profile->history_id));
    return result;
}

static GmailLabel * labelFromLibetpan(struct mailgmail_label * label)
{
    GmailLabel * result = new GmailLabel();
    result->autorelease();
    result->setIdentifier(stringFromNullableCString(label->id));
    result->setName(stringFromNullableCString(label->name));
    result->setType(stringFromNullableCString(label->type));
    result->setMessageListVisibility(stringFromNullableCString(label->message_list_visibility));
    result->setLabelListVisibility(stringFromNullableCString(label->label_list_visibility));
    result->setMessagesTotal(label->messages_total);
    result->setMessagesUnread(label->messages_unread);
    result->setThreadsTotal(label->threads_total);
    result->setThreadsUnread(label->threads_unread);
    return result;
}

static GmailMessageSummary * summaryFromLibetpan(struct mailgmail_message_summary * summary)
{
    GmailMessageSummary * result = new GmailMessageSummary();
    result->autorelease();
    result->setIdentifier(stringFromNullableCString(summary->id));
    result->setThreadID(stringFromNullableCString(summary->thread_id));
    return result;
}

static GmailMessageList * messageListFromLibetpan(struct mailgmail_message_list * list)
{
    GmailMessageList * result = new GmailMessageList();
    result->autorelease();

    Array * messages = Array::array();
    if (list->messages != NULL) {
        for (clistiter * cur = clist_begin(list->messages); cur != NULL; cur = clist_next(cur)) {
            struct mailgmail_message_summary * summary =
                (struct mailgmail_message_summary *) clist_content(cur);
            messages->addObject(summaryFromLibetpan(summary));
        }
    }
    result->setMessages(messages);
    result->setNextPageToken(stringFromNullableCString(list->next_page_token));
    result->setResultSizeEstimate(list->result_size_estimate);
    return result;
}

static GmailMessageHeader * headerFromLibetpan(struct mailgmail_message_header * header)
{
    GmailMessageHeader * result = new GmailMessageHeader();
    result->autorelease();
    result->setName(stringFromNullableCString(header->name));
    result->setValue(stringFromNullableCString(header->value));
    return result;
}

static GmailMessagePart * partFromLibetpan(struct mailgmail_message_part * part,
                                           ErrorCode * pError)
{
    if (part == NULL)
        return NULL;

    GmailMessagePart * result = new GmailMessagePart();
    result->autorelease();
    result->setPartID(stringFromNullableCString(part->part_id));
    result->setMimeType(stringFromNullableCString(part->mime_type));
    result->setFilename(stringFromNullableCString(part->filename));

    Array * headers = Array::array();
    if (part->headers != NULL) {
        for (clistiter * cur = clist_begin(part->headers); cur != NULL; cur = clist_next(cur)) {
            struct mailgmail_message_header * header =
                (struct mailgmail_message_header *) clist_content(cur);
            headers->addObject(headerFromLibetpan(header));
        }
    }
    result->setHeaders(headers);
    if (part->body != NULL) {
        result->setAttachmentID(stringFromNullableCString(part->body->attachment_id));
        result->setSize(part->body->size);
        result->setData(decodedBase64URLDataFromNullableCString(part->body->data, pError));
    }

    Array * parts = Array::array();
    if (part->parts != NULL) {
        for (clistiter * cur = clist_begin(part->parts); cur != NULL; cur = clist_next(cur)) {
            struct mailgmail_message_part * child =
                (struct mailgmail_message_part *) clist_content(cur);
            GmailMessagePart * converted = partFromLibetpan(child, pError);
            if (converted != NULL) {
                parts->addObject(converted);
            }
        }
    }
    result->setParts(parts);
    return result;
}

static GmailMessage * messageFromLibetpan(struct mailgmail_message * message,
                                          ErrorCode * pError)
{
    GmailMessage * result = new GmailMessage();
    result->autorelease();
    result->setIdentifier(stringFromNullableCString(message->id));
    result->setThreadID(stringFromNullableCString(message->thread_id));
    result->setLabelIDs(stringArrayFromCList(message->label_ids));
    result->setSnippet(stringFromNullableCString(message->snippet));
    result->setHistoryID(stringFromNullableCString(message->history_id));
    result->setInternalDate(stringFromNullableCString(message->internal_date));
    result->setSizeEstimate(message->size_estimate);
    result->setRFC822Data(decodedBase64URLDataFromNullableCString(message->raw, pError));
    result->setPayload(partFromLibetpan(message->payload, pError));
    return result;
}

static struct mailgmail_message_list_request *
createLibetpanMessageListRequest(GmailMessageListRequest * request, ErrorCode * pError)
{
    struct mailgmail_message_list_request * result;
    int r;

    result = mailgmail_message_list_request_new();
    if (result == NULL) {
        * pError = ErrorConnection;
        return NULL;
    }

    result->max_results = request->maxResults;
    result->include_spam_trash = request->includeSpamTrash;

    if (request->pageToken != NULL) {
        r = mailgmail_message_list_request_set_page_token(result,
                                                          request->pageToken->UTF8Characters());
        if (r != MAILGMAIL_NO_ERROR)
            goto error;
    }

    if (request->query != NULL) {
        r = mailgmail_message_list_request_set_query(result, request->query->UTF8Characters());
        if (r != MAILGMAIL_NO_ERROR)
            goto error;
    }

    if (request->labelIDs != NULL) {
        for (unsigned int i = 0; i < request->labelIDs->count(); i ++) {
            String * labelID = (String *) request->labelIDs->objectAtIndex(i);
            r = mailgmail_message_list_request_add_label_id(result, labelID->UTF8Characters());
            if (r != MAILGMAIL_NO_ERROR)
                goto error;
        }
    }

    * pError = ErrorNone;
    return result;

error:
    * pError = errorCodeFromGmailError(r);
    mailgmail_message_list_request_free(result);
    return NULL;
}

static struct mailgmail_message_get_request *
createLibetpanMessageGetRequest(GmailMessageGetRequest * request, ErrorCode * pError)
{
    struct mailgmail_message_get_request * result;
    int r;

    result = mailgmail_message_get_request_new(mailgmailFormatFromGmailFormat(request->format));
    if (result == NULL) {
        * pError = ErrorConnection;
        return NULL;
    }

    if ((request->format == GmailMessageFormatMetadata) && (request->metadataHeaders != NULL)) {
        for (unsigned int i = 0; i < request->metadataHeaders->count(); i ++) {
            String * header = (String *) request->metadataHeaders->objectAtIndex(i);
            r = mailgmail_message_get_request_add_metadata_header(result,
                                                                  header->UTF8Characters());
            if (r != MAILGMAIL_NO_ERROR) {
                * pError = errorCodeFromGmailError(r);
                mailgmail_message_get_request_free(result);
                return NULL;
            }
        }
    }

    * pError = ErrorNone;
    return result;
}

void GmailSession::init()
{
    mUserID = NULL;
    mOAuth2Token = NULL;
    mUserAgent = NULL;
    mTimeout = 60;
    mGmail = NULL;
}

GmailSession::GmailSession()
{
    init();
}

GmailSession::~GmailSession()
{
    unsetup();
    MC_SAFE_RELEASE(mUserID);
    MC_SAFE_RELEASE(mOAuth2Token);
    MC_SAFE_RELEASE(mUserAgent);
}

void GmailSession::setUserID(String * userID)
{
    MC_SAFE_REPLACE_COPY(String, mUserID, userID);
    if (mGmail != NULL) {
        mailgmail_set_user(mGmail, mUserID != NULL ? mUserID->UTF8Characters() : NULL);
    }
}

String * GmailSession::userID()
{
    return mUserID;
}

void GmailSession::setOAuth2Token(String * token)
{
    MC_SAFE_REPLACE_COPY(String, mOAuth2Token, token);
    if (mGmail != NULL) {
        mailgmail_set_oauth2_token(mGmail, mOAuth2Token != NULL ? mOAuth2Token->UTF8Characters() : NULL);
    }
}

String * GmailSession::OAuth2Token()
{
    return mOAuth2Token;
}

void GmailSession::setUserAgent(String * userAgent)
{
    MC_SAFE_REPLACE_COPY(String, mUserAgent, userAgent);
    if (mGmail != NULL) {
        mailgmail_set_user_agent(mGmail, mUserAgent != NULL ? mUserAgent->UTF8Characters() : NULL);
    }
}

String * GmailSession::userAgent()
{
    return mUserAgent;
}

void GmailSession::setTimeout(time_t timeout)
{
    mTimeout = timeout;
    if (mGmail != NULL) {
        mailgmail_set_timeout(mGmail, mTimeout);
    }
}

time_t GmailSession::timeout()
{
    return mTimeout;
}

void GmailSession::setup(ErrorCode * pError)
{
    if (mGmail != NULL) {
        * pError = ErrorNone;
        return;
    }

    mGmail = mailgmail_new();
    if (mGmail == NULL) {
        * pError = ErrorConnection;
        return;
    }

    applyConfiguration(pError);
    if (* pError != ErrorNone) {
        unsetup();
    }
}

void GmailSession::unsetup()
{
    if (mGmail != NULL) {
        mailgmail_free(mGmail);
        mGmail = NULL;
    }
}

bool GmailSession::isSetup()
{
    return mGmail != NULL;
}

void GmailSession::applyConfiguration(ErrorCode * pError)
{
    int r;

    r = mailgmail_set_user(mGmail, mUserID != NULL ? mUserID->UTF8Characters() : NULL);
    if (r != MAILGMAIL_NO_ERROR)
        goto error;

    r = mailgmail_set_oauth2_token(mGmail,
                                   mOAuth2Token != NULL ? mOAuth2Token->UTF8Characters() : NULL);
    if (r != MAILGMAIL_NO_ERROR)
        goto error;

    r = mailgmail_set_user_agent(mGmail,
                                 mUserAgent != NULL ? mUserAgent->UTF8Characters() : NULL);
    if (r != MAILGMAIL_NO_ERROR)
        goto error;

    r = mailgmail_set_timeout(mGmail, mTimeout);
    if (r != MAILGMAIL_NO_ERROR)
        goto error;

    * pError = ErrorNone;
    return;

error:
    * pError = errorCodeFromGmailError(r);
}

GmailProfile * GmailSession::profile(ErrorCode * pError)
{
    struct mailgmail_profile * profile;
    int r;

    setup(pError);
    if (* pError != ErrorNone)
        return NULL;

    profile = NULL;
    r = mailgmail_get_profile(mGmail, &profile);
    if (r != MAILGMAIL_NO_ERROR) {
        * pError = errorCodeFromGmailError(r);
        return NULL;
    }

    GmailProfile * result = profileFromLibetpan(profile);
    mailgmail_profile_free(profile);
    * pError = ErrorNone;
    return result;
}

Array * GmailSession::labels(ErrorCode * pError)
{
    struct mailgmail_label_list * labelList;
    int r;

    setup(pError);
    if (* pError != ErrorNone)
        return NULL;

    labelList = NULL;
    r = mailgmail_list_labels(mGmail, &labelList);
    if (r != MAILGMAIL_NO_ERROR) {
        * pError = errorCodeFromGmailError(r);
        return NULL;
    }

    Array * result = Array::array();
    if (labelList->labels != NULL) {
        for (clistiter * cur = clist_begin(labelList->labels); cur != NULL; cur = clist_next(cur)) {
            struct mailgmail_label * label = (struct mailgmail_label *) clist_content(cur);
            result->addObject(labelFromLibetpan(label));
        }
    }
    mailgmail_label_list_free(labelList);
    * pError = ErrorNone;
    return result;
}

GmailLabel * GmailSession::label(String * labelID, ErrorCode * pError)
{
    struct mailgmail_label * gmailLabel;
    int r;

    setup(pError);
    if (* pError != ErrorNone)
        return NULL;

    gmailLabel = NULL;
    r = mailgmail_get_label(mGmail, labelID->UTF8Characters(), &gmailLabel);
    if (r != MAILGMAIL_NO_ERROR) {
        * pError = errorCodeFromGmailError(r);
        return NULL;
    }

    GmailLabel * result = labelFromLibetpan(gmailLabel);
    mailgmail_label_free(gmailLabel);
    * pError = ErrorNone;
    return result;
}

GmailMessageList * GmailSession::messages(ErrorCode * pError)
{
    return messagesWithQuery(NULL, pError);
}

GmailMessageList * GmailSession::messagesWithQuery(String * query, ErrorCode * pError)
{
    GmailMessageListRequest request;
    MC_SAFE_REPLACE_RETAIN(String, request.query, query);

    setup(pError);
    if (* pError != ErrorNone)
        return NULL;

    struct mailgmail_message_list_request * lepRequest =
        createLibetpanMessageListRequest(&request, pError);
    if (* pError != ErrorNone)
        return NULL;

    struct mailgmail_message_list * list = NULL;
    int r = mailgmail_list_messages(mGmail, lepRequest, &list);
    mailgmail_message_list_request_free(lepRequest);
    if (r != MAILGMAIL_NO_ERROR) {
        * pError = errorCodeFromGmailError(r);
        return NULL;
    }

    GmailMessageList * result = messageListFromLibetpan(list);
    mailgmail_message_list_free(list);
    * pError = ErrorNone;
    return result;
}

GmailMessageList * GmailSession::messagesWithLabel(String * labelID, ErrorCode * pError)
{
    GmailMessageListRequest request;
    request.labelIDs = (Array *) Array::arrayWithObject(labelID)->retain();

    setup(pError);
    if (* pError != ErrorNone)
        return NULL;

    struct mailgmail_message_list_request * lepRequest =
        createLibetpanMessageListRequest(&request, pError);
    if (* pError != ErrorNone)
        return NULL;

    struct mailgmail_message_list * list = NULL;
    int r = mailgmail_list_messages(mGmail, lepRequest, &list);
    mailgmail_message_list_request_free(lepRequest);
    if (r != MAILGMAIL_NO_ERROR) {
        * pError = errorCodeFromGmailError(r);
        return NULL;
    }

    GmailMessageList * result = messageListFromLibetpan(list);
    mailgmail_message_list_free(list);
    * pError = ErrorNone;
    return result;
}

GmailMessage * GmailSession::message(String * messageID, ErrorCode * pError)
{
    return messageWithFormat(messageID, GmailMessageFormatFull, pError);
}

GmailMessage * GmailSession::messageWithFormat(String * messageID, GmailMessageFormat format,
                                               ErrorCode * pError)
{
    GmailMessageGetRequest request;
    request.format = format;

    setup(pError);
    if (* pError != ErrorNone)
        return NULL;

    struct mailgmail_message_get_request * lepRequest =
        createLibetpanMessageGetRequest(&request, pError);
    if (* pError != ErrorNone)
        return NULL;

    struct mailgmail_message * message = NULL;
    int r = mailgmail_get_message(mGmail, messageID->UTF8Characters(), lepRequest, &message);
    mailgmail_message_get_request_free(lepRequest);
    if (r != MAILGMAIL_NO_ERROR) {
        * pError = errorCodeFromGmailError(r);
        return NULL;
    }

    GmailMessage * result = messageFromLibetpan(message, pError);
    mailgmail_message_free(message);
    if (* pError != ErrorNone)
        return NULL;
    * pError = ErrorNone;
    return result;
}

GmailMessage * GmailSession::messageWithMetadataHeaders(String * messageID, Array * headers,
                                                        ErrorCode * pError)
{
    GmailMessageGetRequest request;
    request.format = GmailMessageFormatMetadata;
    MC_SAFE_REPLACE_RETAIN(Array, request.metadataHeaders, headers);

    setup(pError);
    if (* pError != ErrorNone)
        return NULL;

    struct mailgmail_message_get_request * lepRequest =
        createLibetpanMessageGetRequest(&request, pError);
    if (* pError != ErrorNone)
        return NULL;

    struct mailgmail_message * message = NULL;
    int r = mailgmail_get_message(mGmail, messageID->UTF8Characters(), lepRequest, &message);
    mailgmail_message_get_request_free(lepRequest);
    if (r != MAILGMAIL_NO_ERROR) {
        * pError = errorCodeFromGmailError(r);
        return NULL;
    }

    GmailMessage * result = messageFromLibetpan(message, pError);
    mailgmail_message_free(message);
    if (* pError != ErrorNone)
        return NULL;
    * pError = ErrorNone;
    return result;
}

Data * GmailSession::messageData(String * messageID, ErrorCode * pError)
{
    GmailMessage * gmailMessage = messageWithFormat(messageID, GmailMessageFormatRaw, pError);
    if (* pError != ErrorNone)
        return NULL;

    Data * result = gmailMessage->RFC822Data();
    if (result != NULL) {
        result->retain()->autorelease();
    }
    return result;
}

Data * GmailSession::attachmentData(String * messageID, String * attachmentID,
                                    ErrorCode * pError)
{
    struct mailgmail_attachment * gmailAttachment;
    int r;

    setup(pError);
    if (* pError != ErrorNone)
        return NULL;

    gmailAttachment = NULL;
    r = mailgmail_get_attachment(mGmail, messageID->UTF8Characters(),
                                 attachmentID->UTF8Characters(), &gmailAttachment);
    if (r != MAILGMAIL_NO_ERROR) {
        * pError = errorCodeFromGmailError(r);
        return NULL;
    }

    Data * result = decodedBase64URLDataFromNullableCString(gmailAttachment->data, pError);
    if (result != NULL) {
        result->retain()->autorelease();
    }
    mailgmail_attachment_free(gmailAttachment);
    if (* pError != ErrorNone)
        return NULL;
    * pError = ErrorNone;
    return result;
}

Data * GmailSession::dataForMessagePart(String * messageID, GmailMessagePart * part,
                                        ErrorCode * pError)
{
    if (part->data() != NULL) {
        Data * result = part->data();
        result->retain()->autorelease();
        * pError = ErrorNone;
        return result;
    }

    if (part->attachmentID() != NULL) {
        return attachmentData(messageID, part->attachmentID(), pError);
    }

    * pError = ErrorFetch;
    return NULL;
}

int GmailSession::lastHTTPStatus()
{
    if (mGmail == NULL)
        return 0;

    return mailgmail_get_last_http_status(mGmail);
}

String * GmailSession::lastErrorMessage()
{
    if (mGmail == NULL)
        return NULL;

    return stringFromNullableCString(mailgmail_get_last_error_message(mGmail));
}
