#include "MCJMAPSession.h"

#include <libetpan/mailjmap.h>
#include <libetpan/clist.h>
#include <libetpan/chash.h>
#include <string.h>
#include <stdlib.h>

#include "MCAddress.h"
#include "MCJMAPBlobUpload.h"
#include "MCJMAPMailbox.h"
#include "MCJMAPMessage.h"
#include "MCJMAPMessagePart.h"
#include "MCJMAPMultipart.h"
#include "MCJMAPPart.h"
#include "MCJMAPSubmission.h"
#include "MCMessageHeader.h"
#include "MCValue.h"

using namespace mailcore;

#define JMAP_MAIL_CAPABILITY "urn:ietf:params:jmap:mail"

static ErrorCode errorCodeFromJMAPError(int r)
{
    switch (r) {
        case MAILJMAP_NO_ERROR:
            return ErrorNone;
        case MAILJMAP_ERROR_AUTHENTICATION:
            return ErrorAuthentication;
        case MAILJMAP_ERROR_JSON_PARSE:
        case MAILJMAP_ERROR_PROTOCOL:
            return ErrorParse;
        case MAILJMAP_ERROR_CAPABILITY:
            return ErrorCapability;
        case MAILJMAP_ERROR_METHOD:
            return ErrorFetch;
        case MAILJMAP_ERROR_MEMORY:
        case MAILJMAP_ERROR_BAD_STATE:
        case MAILJMAP_ERROR_DISCOVERY:
        case MAILJMAP_ERROR_HTTP:
        case MAILJMAP_ERROR_LIMIT:
        case MAILJMAP_ERROR_STREAM:
        default:
            return ErrorConnection;
    }
}

static String * stringFromNullableCString(const char * value)
{
    if (value == NULL)
        return NULL;
    return String::stringWithUTF8Characters(value);
}

static const char * utf8(String * value)
{
    if (value == NULL)
        return NULL;
    return value->UTF8Characters();
}

static Array * stringArrayFromCList(clist * list)
{
    Array * result = Array::array();
    if (list == NULL)
        return result;

    for (clistiter * cur = clist_begin(list); cur != NULL; cur = clist_next(cur)) {
        char * value = (char *) clist_content(cur);
        if (value != NULL)
            result->addObject(String::stringWithUTF8Characters(value));
    }
    return result;
}

static clist * cListFromStringArray(Array * array)
{
    clist * result = clist_new();
    if (result == NULL)
        return NULL;
    if (array == NULL)
        return result;

    for (unsigned int i = 0; i < array->count(); i ++) {
        String * value = (String *) array->objectAtIndex(i);
        if (value != NULL)
            clist_append(result, strdup(value->UTF8Characters()));
    }
    return result;
}

static void freeStringCList(clist * list)
{
    if (list == NULL)
        return;
    for (clistiter * cur = clist_begin(list); cur != NULL; cur = clist_next(cur)) {
        free(clist_content(cur));
    }
    clist_free(list);
}

static Array * stringArrayFromHashKeys(chash * hash)
{
    Array * result = Array::array();
    if (hash == NULL)
        return result;

    for (chashiter * cur = chash_begin(hash); cur != NULL; cur = chash_next(hash, cur)) {
        chashdatum key;
        chash_key(cur, &key);
        if (key.data != NULL)
            result->addObject(String::stringWithUTF8Characters((char *) key.data));
    }
    return result;
}

static HashMap * boolHashMapFromHash(chash * hash)
{
    HashMap * result = HashMap::hashMap();
    if (hash == NULL)
        return result;

    for (chashiter * cur = chash_begin(hash); cur != NULL; cur = chash_next(hash, cur)) {
        chashdatum key;
        chashdatum value;
        chash_key(cur, &key);
        chash_value(cur, &value);
        if (key.data != NULL) {
            int enabled = value.data != NULL ? *((int *) value.data) : 0;
            result->setObjectForKey(String::stringWithUTF8Characters((char *) key.data),
                                    Value::valueWithBoolValue(enabled != 0));
        }
    }
    return result;
}

static Address * addressFromJMAP(struct mailjmap_email_address * address)
{
    if (address == NULL)
        return NULL;
    return Address::addressWithDisplayName(stringFromNullableCString(address->name),
                                           stringFromNullableCString(address->email));
}

static Array * addressArrayFromCList(clist * list)
{
    Array * result = Array::array();
    if (list == NULL)
        return result;

    for (clistiter * cur = clist_begin(list); cur != NULL; cur = clist_next(cur)) {
        Address * address = addressFromJMAP((struct mailjmap_email_address *) clist_content(cur));
        if (address != NULL)
            result->addObject(address);
    }
    return result;
}

static Address * firstAddressFromCList(clist * list)
{
    if ((list == NULL) || (clist_begin(list) == NULL))
        return NULL;
    return addressFromJMAP((struct mailjmap_email_address *) clist_content(clist_begin(list)));
}

static bool hasPrefixCaseInsensitive(const char * value, const char * prefix)
{
    if (value == NULL)
        return false;
    return strncasecmp(value, prefix, strlen(prefix)) == 0;
}

static PartType partTypeForMultipart(const char * mimeType)
{
    if (mimeType == NULL)
        return PartTypeMultipartMixed;
    if (strcasecmp(mimeType, "multipart/alternative") == 0)
        return PartTypeMultipartAlternative;
    if (strcasecmp(mimeType, "multipart/related") == 0)
        return PartTypeMultipartRelated;
    if (strcasecmp(mimeType, "multipart/signed") == 0)
        return PartTypeMultipartSigned;
    return PartTypeMultipartMixed;
}

static void importBodyPartFields(AbstractPart * result, struct mailjmap_email_body_part * part)
{
    if ((result == NULL) || (part == NULL))
        return;

    result->setMimeType(stringFromNullableCString(part->type));
    result->setFilename(stringFromNullableCString(part->name));
    result->setCharset(stringFromNullableCString(part->charset));
    result->setContentID(stringFromNullableCString(part->cid));
    result->setContentLocation(stringFromNullableCString(part->location));
    if (part->disposition != NULL) {
        if (strcasecmp(part->disposition, "attachment") == 0)
            result->setAttachment(true);
        else if (strcasecmp(part->disposition, "inline") == 0)
            result->setInlineAttachment(true);
    }
}

static AbstractPart * bodyPartFromJMAP(struct mailjmap_email_body_part * part)
{
    if (part == NULL)
        return NULL;

    const char * mimeType = part->type != NULL ? part->type : "application/octet-stream";
    if (hasPrefixCaseInsensitive(mimeType, "multipart/")) {
        JMAPMultipart * result = new JMAPMultipart();
        result->autorelease();
        result->setPartID(stringFromNullableCString(part->part_id));
        result->setBlobID(stringFromNullableCString(part->blob_id));
        if (part->has_size)
            result->setSize((unsigned int) part->size);
        result->setLanguage(stringFromNullableCString(part->language));
        result->setLanguages(stringArrayFromCList(part->languages));
        result->setPartType(partTypeForMultipart(mimeType));
        importBodyPartFields(result, part);

        Array * parts = Array::array();
        if (part->sub_parts != NULL) {
            for (clistiter * cur = clist_begin(part->sub_parts); cur != NULL; cur = clist_next(cur)) {
                AbstractPart * child = bodyPartFromJMAP((struct mailjmap_email_body_part *) clist_content(cur));
                if (child != NULL)
                    parts->addObject(child);
            }
        }
        result->setParts(parts);
        return result;
    }

    if (strcasecmp(mimeType, "message/rfc822") == 0) {
        JMAPMessagePart * result = new JMAPMessagePart();
        result->autorelease();
        result->setPartID(stringFromNullableCString(part->part_id));
        result->setBlobID(stringFromNullableCString(part->blob_id));
        if (part->has_size)
            result->setSize((unsigned int) part->size);
        result->setPartType(PartTypeMessage);
        importBodyPartFields(result, part);
        if ((part->sub_parts != NULL) && (clist_begin(part->sub_parts) != NULL)) {
            result->setMainPart(bodyPartFromJMAP((struct mailjmap_email_body_part *) clist_content(clist_begin(part->sub_parts))));
        }
        return result;
    }

    JMAPPart * result = new JMAPPart();
    result->autorelease();
    result->setPartID(stringFromNullableCString(part->part_id));
    result->setBlobID(stringFromNullableCString(part->blob_id));
    if (part->has_size)
        result->setSize((unsigned int) part->size);
    result->setLanguage(stringFromNullableCString(part->language));
    result->setLanguages(stringArrayFromCList(part->languages));
    result->setPartType(PartTypeSingle);
    importBodyPartFields(result, part);
    return result;
}

static Array * bodyPartArrayFromCList(clist * list)
{
    Array * result = Array::array();
    if (list == NULL)
        return result;

    for (clistiter * cur = clist_begin(list); cur != NULL; cur = clist_next(cur)) {
        AbstractPart * part = bodyPartFromJMAP((struct mailjmap_email_body_part *) clist_content(cur));
        if (part != NULL)
            result->addObject(part);
    }
    return result;
}

static JMAPMailbox * mailboxFromJMAP(struct mailjmap_mailbox * mailbox)
{
    JMAPMailbox * result = new JMAPMailbox();
    result->autorelease();
    result->setIdentifier(stringFromNullableCString(mailbox->id));
    result->setName(stringFromNullableCString(mailbox->name));
    result->setParentID(stringFromNullableCString(mailbox->parent_id));
    result->setRole(stringFromNullableCString(mailbox->role));
    result->setSortOrder(mailbox->sort_order);
    result->setSubscribed(mailbox->is_subscribed != 0);
    result->setTotalEmails(mailbox->total_emails);
    result->setUnreadEmails(mailbox->unread_emails);
    result->setTotalThreads(mailbox->total_threads);
    result->setUnreadThreads(mailbox->unread_threads);
    result->setRights(boolHashMapFromHash(mailbox->my_rights));
    return result;
}

static JMAPMessage * messageFromJMAP(struct mailjmap_email * email)
{
    JMAPMessage * result = new JMAPMessage();
    result->autorelease();
    result->setIdentifier(stringFromNullableCString(email->id));
    result->setBlobID(stringFromNullableCString(email->blob_id));
    result->setThreadID(stringFromNullableCString(email->thread_id));
    result->setMailboxIDs(stringArrayFromHashKeys(email->mailbox_ids));
    result->setKeywords(stringArrayFromHashKeys(email->keywords));
    result->setSize((unsigned int) email->size);
    result->setPreview(stringFromNullableCString(email->preview));

    MessageHeader * header = result->header();
    header->setMessageID(stringFromNullableCString(email->message_id));
    header->setReferences(stringArrayFromCList(email->references));
    header->setInReplyTo(stringArrayFromCList(email->in_reply_to_list));
    header->setSender(firstAddressFromCList(email->sender));
    header->setFrom(firstAddressFromCList(email->from));
    header->setTo(addressArrayFromCList(email->to));
    header->setCc(addressArrayFromCList(email->cc));
    header->setBcc(addressArrayFromCList(email->bcc));
    header->setReplyTo(addressArrayFromCList(email->reply_to));
    header->setSubject(stringFromNullableCString(email->subject));

    result->setMainPart(bodyPartFromJMAP(email->body_structure));
    result->setTextBody(bodyPartArrayFromCList(email->text_body));
    result->setHTMLBody(bodyPartArrayFromCList(email->html_body));
    result->setAttachments(bodyPartArrayFromCList(email->attachments));
    if (result->mainPart() != NULL)
        result->mainPart()->applyUniquePartID();
    return result;
}

static JMAPBlobUpload * blobUploadFromJMAP(struct mailjmap_blob_upload * upload)
{
    JMAPBlobUpload * result = new JMAPBlobUpload();
    result->autorelease();
    result->setAccountID(stringFromNullableCString(upload->account_id));
    result->setBlobID(stringFromNullableCString(upload->blob_id));
    result->setType(stringFromNullableCString(upload->type));
    result->setName(stringFromNullableCString(upload->name));
    result->setSize(upload->size);
    return result;
}

static JMAPSubmission * submissionFromSetCreated(struct mailjmap_set_created * created)
{
    JMAPSubmission * result = new JMAPSubmission();
    result->autorelease();
    result->setIdentifier(stringFromNullableCString(created->id));
    result->setEmailID(stringFromNullableCString(created->email_id));
    result->setIdentityID(stringFromNullableCString(created->identity_id));
    result->setThreadID(stringFromNullableCString(created->thread_id));
    result->setSendAt(stringFromNullableCString(created->send_at));
    result->setUndoStatus(stringFromNullableCString(created->undo_status));
    return result;
}

void JMAPSession::init()
{
    mSessionURL = NULL;
    mDomainOrEmail = NULL;
    mAccountID = NULL;
    mUsername = NULL;
    mOAuth2Token = NULL;
    mTimeout = 60;
    mCheckCertificateEnabled = true;
    mJMAP = NULL;
}

JMAPSession::JMAPSession()
{
    init();
}

JMAPSession::~JMAPSession()
{
    unsetup();
    MC_SAFE_RELEASE(mSessionURL);
    MC_SAFE_RELEASE(mDomainOrEmail);
    MC_SAFE_RELEASE(mAccountID);
    MC_SAFE_RELEASE(mUsername);
    MC_SAFE_RELEASE(mOAuth2Token);
}

void JMAPSession::setSessionURL(String * sessionURL) { MC_SAFE_REPLACE_COPY(String, mSessionURL, sessionURL); }
String * JMAPSession::sessionURL() { return mSessionURL; }
void JMAPSession::setDomainOrEmail(String * domainOrEmail) { MC_SAFE_REPLACE_COPY(String, mDomainOrEmail, domainOrEmail); }
String * JMAPSession::domainOrEmail() { return mDomainOrEmail; }
void JMAPSession::setAccountID(String * accountID) { MC_SAFE_REPLACE_COPY(String, mAccountID, accountID); }
String * JMAPSession::accountID() { return mAccountID; }
void JMAPSession::setUsername(String * username) { MC_SAFE_REPLACE_COPY(String, mUsername, username); }
String * JMAPSession::username() { return mUsername; }
void JMAPSession::setOAuth2Token(String * token) { MC_SAFE_REPLACE_COPY(String, mOAuth2Token, token); }
String * JMAPSession::OAuth2Token() { return mOAuth2Token; }
void JMAPSession::setTimeout(time_t timeout) { mTimeout = timeout; }
time_t JMAPSession::timeout() { return mTimeout; }
void JMAPSession::setCheckCertificateEnabled(bool enabled) { mCheckCertificateEnabled = enabled; }
bool JMAPSession::isCheckCertificateEnabled() { return mCheckCertificateEnabled; }

void JMAPSession::setup(ErrorCode * pError)
{
    if (mJMAP != NULL) {
        * pError = ErrorNone;
        return;
    }
    mJMAP = mailjmap_new(0, NULL);
    if (mJMAP == NULL) {
        * pError = ErrorConnection;
        return;
    }
    mailjmap_set_timeout(mJMAP, mTimeout);
    * pError = ErrorNone;
}

void JMAPSession::unsetup()
{
    if (mJMAP != NULL) {
        mailjmap_free(mJMAP);
        mJMAP = NULL;
    }
}

bool JMAPSession::isSetup()
{
    return mJMAP != NULL;
}

bool JMAPSession::checkCertificate()
{
    // libetpan's JMAP HTTP transport needs to expose a TLS verification hook
    // before MailCore can mirror IMAP's mailstream-based certificate check here.
    return true;
}

void JMAPSession::connect(ErrorCode * pError)
{
    setup(pError);
    if (* pError != ErrorNone)
        return;
    int r = mailjmap_connect(mJMAP, utf8(mSessionURL));
    * pError = errorCodeFromJMAPError(r);
    if (* pError == ErrorNone && !checkCertificate())
        * pError = ErrorCertificate;
}

void JMAPSession::discover(ErrorCode * pError)
{
    setup(pError);
    if (* pError != ErrorNone)
        return;
    struct mailjmap_session * session = NULL;
    int r = mailjmap_discover(mJMAP, utf8(mDomainOrEmail), &session);
    * pError = errorCodeFromJMAPError(r);
    if (* pError == ErrorNone && session != NULL) {
        setSessionURL(stringFromNullableCString(session->api_url));
        mailjmap_session_free(session);
    }
}

void JMAPSession::login(ErrorCode * pError)
{
    setup(pError);
    if (* pError != ErrorNone)
        return;
    int r = mailjmap_login_oauth2(mJMAP, utf8(mUsername), utf8(mOAuth2Token));
    * pError = errorCodeFromJMAPError(r);
}

void JMAPSession::disconnect()
{
    unsetup();
}

String * JMAPSession::primaryMailAccountID(ErrorCode * pError)
{
    if (mAccountID != NULL) {
        * pError = ErrorNone;
        return mAccountID;
    }
    setup(pError);
    if (* pError != ErrorNone)
        return NULL;

    struct mailjmap_session * session = NULL;
    int r = mailjmap_get_session(mJMAP, &session);
    * pError = errorCodeFromJMAPError(r);
    if (* pError != ErrorNone || session == NULL)
        return NULL;

    String * result = NULL;
    for (clistiter * cur = clist_begin(session->primary_accounts); cur != NULL; cur = clist_next(cur)) {
        struct mailjmap_session_primary_account * account = (struct mailjmap_session_primary_account *) clist_content(cur);
        if ((account->capability != NULL) && (strcmp(account->capability, JMAP_MAIL_CAPABILITY) == 0)) {
            result = stringFromNullableCString(account->account_id);
            break;
        }
    }
    if (result == NULL && clist_begin(session->accounts) != NULL) {
        struct mailjmap_session_account * account = (struct mailjmap_session_account *) clist_content(clist_begin(session->accounts));
        result = stringFromNullableCString(account->account_id);
    }
    mailjmap_session_free(session);
    if (result != NULL)
        setAccountID(result);
    return mAccountID;
}

String * JMAPSession::effectiveAccountID(ErrorCode * pError)
{
    return primaryMailAccountID(pError);
}

Array * JMAPSession::fetchMailboxes(ErrorCode * pError)
{
    String * account = effectiveAccountID(pError);
    if (* pError != ErrorNone)
        return NULL;

    struct mailjmap_mailbox_get_result * mailboxResult = NULL;
    int r = mailjmap_mailbox_get(mJMAP, utf8(account), NULL, NULL, &mailboxResult);
    * pError = errorCodeFromJMAPError(r);
    if (* pError != ErrorNone)
        return NULL;

    Array * result = Array::array();
    if (mailboxResult->list != NULL) {
        for (clistiter * cur = clist_begin(mailboxResult->list); cur != NULL; cur = clist_next(cur)) {
            result->addObject(mailboxFromJMAP((struct mailjmap_mailbox *) clist_content(cur)));
        }
    }
    mailjmap_mailbox_get_result_free(mailboxResult);
    return result;
}

Array * JMAPSession::queryMessagesWithText(String * text, unsigned int position, unsigned int limit, ErrorCode * pError)
{
    String * account = effectiveAccountID(pError);
    if (* pError != ErrorNone)
        return NULL;

    struct mailjmap_query_result * queryResult = NULL;
    int r = mailjmap_email_query_with_text_filter(mJMAP, utf8(account), utf8(text), (int) position, (int) limit, &queryResult);
    * pError = errorCodeFromJMAPError(r);
    if (* pError != ErrorNone)
        return NULL;
    Array * result = stringArrayFromCList(queryResult->ids);
    mailjmap_query_result_free(queryResult);
    return result;
}

Array * JMAPSession::queryMessagesInMailbox(String * mailboxID, unsigned int position, unsigned int limit, ErrorCode * pError)
{
    String * account = effectiveAccountID(pError);
    if (* pError != ErrorNone)
        return NULL;

    struct mailjmap_email_query_filter * filter = mailjmap_email_query_filter_condition_new();
    if (filter == NULL) {
        * pError = ErrorConnection;
        return NULL;
    }
    int r = mailjmap_email_query_filter_set_in_mailbox(filter, utf8(mailboxID));
    if (r != MAILJMAP_NO_ERROR) {
        mailjmap_email_query_filter_free(filter);
        * pError = errorCodeFromJMAPError(r);
        return NULL;
    }
    struct mailjmap_query_result * queryResult = NULL;
    r = mailjmap_email_query_with_filter_options(mJMAP, utf8(account), filter, NULL, (int) position, NULL, 0, (int) limit, 0, 0, &queryResult);
    mailjmap_email_query_filter_free(filter);
    * pError = errorCodeFromJMAPError(r);
    if (* pError != ErrorNone)
        return NULL;
    Array * result = stringArrayFromCList(queryResult->ids);
    mailjmap_query_result_free(queryResult);
    return result;
}

Array * JMAPSession::fetchMessages(Array * ids, Array * properties, ErrorCode * pError)
{
    String * account = effectiveAccountID(pError);
    if (* pError != ErrorNone)
        return NULL;

    clist * idsList = cListFromStringArray(ids);
    clist * propertiesList = cListFromStringArray(properties);
    struct mailjmap_email_get_result * emailResult = NULL;
    int r = mailjmap_email_get(mJMAP, utf8(account), idsList, propertiesList, &emailResult);
    freeStringCList(idsList);
    freeStringCList(propertiesList);
    * pError = errorCodeFromJMAPError(r);
    if (* pError != ErrorNone)
        return NULL;

    Array * result = Array::array();
    if (emailResult->list != NULL) {
        for (clistiter * cur = clist_begin(emailResult->list); cur != NULL; cur = clist_next(cur)) {
            result->addObject(messageFromJMAP((struct mailjmap_email *) clist_content(cur)));
        }
    }
    mailjmap_email_get_result_free(emailResult);
    return result;
}

JMAPBlobUpload * JMAPSession::upload(Data * data, String * contentType, String * accountID, ErrorCode * pError)
{
    String * account = accountID != NULL ? accountID : effectiveAccountID(pError);
    if (* pError != ErrorNone)
        return NULL;
    struct mailjmap_blob_upload * uploadResult = NULL;
    int r = mailjmap_upload(mJMAP, utf8(account), utf8(contentType), data->bytes(), data->length(), &uploadResult);
    * pError = errorCodeFromJMAPError(r);
    if (* pError != ErrorNone)
        return NULL;
    JMAPBlobUpload * result = blobUploadFromJMAP(uploadResult);
    mailjmap_blob_upload_free(uploadResult);
    return result;
}

Data * JMAPSession::download(String * blobID, String * name, String * accept, String * accountID, ErrorCode * pError)
{
    String * account = accountID != NULL ? accountID : effectiveAccountID(pError);
    if (* pError != ErrorNone)
        return NULL;
    char * bytes = NULL;
    size_t length = 0;
    int r = mailjmap_download(mJMAP, utf8(account), utf8(blobID), utf8(name), utf8(accept), &bytes, &length);
    * pError = errorCodeFromJMAPError(r);
    if (* pError != ErrorNone)
        return NULL;
    Data * result = Data::dataWithBytes(bytes, (unsigned int) length);
    free(bytes);
    return result;
}

JMAPMessage * JMAPSession::createDraft(Data * rfc822Data, String * mailboxID, Array * keywords, ErrorCode * pError)
{
    JMAPBlobUpload * uploadResult = upload(rfc822Data, MCSTR("message/rfc822"), NULL, pError);
    if (* pError != ErrorNone)
        return NULL;
    String * account = effectiveAccountID(pError);
    if (* pError != ErrorNone)
        return NULL;
    clist * mailboxIDs = clist_new();
    if (mailboxID != NULL)
        clist_append(mailboxIDs, strdup(mailboxID->UTF8Characters()));
    clist * keywordList = cListFromStringArray(keywords);
    struct mailjmap_import_result * importResult = NULL;
    int r = mailjmap_email_import(mJMAP, utf8(account), "draft", utf8(uploadResult->blobID()), mailboxIDs, keywordList, NULL, &importResult);
    freeStringCList(mailboxIDs);
    freeStringCList(keywordList);
    * pError = errorCodeFromJMAPError(r);
    if (* pError != ErrorNone)
        return NULL;

    JMAPMessage * result = NULL;
    if ((importResult->created != NULL) && (clist_begin(importResult->created) != NULL)) {
        struct mailjmap_import_created * created = (struct mailjmap_import_created *) clist_content(clist_begin(importResult->created));
        if (created->email != NULL)
            result = messageFromJMAP(created->email);
    }
    mailjmap_import_result_free(importResult);
    return result;
}

void JMAPSession::updateDraft(String * messageID, Array * mailboxIDs, Array * keywords, ErrorCode * pError)
{
    String * account = effectiveAccountID(pError);
    if (* pError != ErrorNone)
        return;
    struct mailjmap_email_set_item * item = mailjmap_email_set_item_new(utf8(messageID));
    if (item == NULL) {
        * pError = ErrorConnection;
        return;
    }
    if (mailboxIDs != NULL) {
        for (unsigned int i = 0; i < mailboxIDs->count(); i ++)
            mailjmap_email_set_item_add_mailbox_id(item, utf8((String *) mailboxIDs->objectAtIndex(i)));
    }
    if (keywords != NULL) {
        for (unsigned int i = 0; i < keywords->count(); i ++)
            mailjmap_email_set_item_add_keyword(item, utf8((String *) keywords->objectAtIndex(i)));
    }
    clist * update = clist_new();
    clist_append(update, item);
    struct mailjmap_set_result * setResult = NULL;
    int r = mailjmap_email_set(mJMAP, utf8(account), NULL, NULL, update, NULL, &setResult);
    clist_free(update);
    mailjmap_email_set_item_free(item);
    if (setResult != NULL)
        mailjmap_set_result_free(setResult);
    * pError = errorCodeFromJMAPError(r);
}

void JMAPSession::deleteDraft(String * messageID, ErrorCode * pError)
{
    String * account = effectiveAccountID(pError);
    if (* pError != ErrorNone)
        return;
    clist * destroy = clist_new();
    clist_append(destroy, strdup(messageID->UTF8Characters()));
    struct mailjmap_set_result * setResult = NULL;
    int r = mailjmap_email_set(mJMAP, utf8(account), NULL, NULL, NULL, destroy, &setResult);
    freeStringCList(destroy);
    if (setResult != NULL)
        mailjmap_set_result_free(setResult);
    * pError = errorCodeFromJMAPError(r);
}

JMAPSubmission * JMAPSession::sendMessage(String * messageID, String * identityID, ErrorCode * pError)
{
    String * account = effectiveAccountID(pError);
    if (* pError != ErrorNone)
        return NULL;
    struct mailjmap_email_submission_set_item * item = mailjmap_email_submission_set_item_new("send");
    if (item == NULL) {
        * pError = ErrorConnection;
        return NULL;
    }
    mailjmap_email_submission_set_item_set_email_id(item, utf8(messageID));
    mailjmap_email_submission_set_item_set_identity_id(item, utf8(identityID));
    clist * create = clist_new();
    clist_append(create, item);
    struct mailjmap_set_result * setResult = NULL;
    int r = mailjmap_email_submission_set(mJMAP, utf8(account), NULL, create, NULL, NULL, &setResult);
    clist_free(create);
    mailjmap_email_submission_set_item_free(item);
    * pError = errorCodeFromJMAPError(r);
    if (* pError != ErrorNone)
        return NULL;
    JMAPSubmission * result = NULL;
    if ((setResult->created != NULL) && (clist_begin(setResult->created) != NULL)) {
        result = submissionFromSetCreated((struct mailjmap_set_created *) clist_content(clist_begin(setResult->created)));
    }
    mailjmap_set_result_free(setResult);
    return result;
}

JMAPSubmission * JMAPSession::sendData(Data * rfc822Data, String * identityID, ErrorCode * pError)
{
    JMAPMessage * draft = createDraft(rfc822Data, NULL, NULL, pError);
    if (* pError != ErrorNone || draft == NULL)
        return NULL;
    return sendMessage(draft->identifier(), identityID, pError);
}

int JMAPSession::lastHTTPStatus()
{
    if (mJMAP == NULL)
        return 0;
    return mailjmap_get_last_http_status(mJMAP);
}

String * JMAPSession::lastErrorMessage()
{
    if (mJMAP == NULL)
        return NULL;
    return stringFromNullableCString(mailjmap_get_last_error_message(mJMAP));
}

String * JMAPSession::lastProblemType()
{
    if (mJMAP == NULL)
        return NULL;
    return stringFromNullableCString(mailjmap_get_last_problem_type(mJMAP));
}

String * JMAPSession::lastMethodErrorType()
{
    if (mJMAP == NULL)
        return NULL;
    return stringFromNullableCString(mailjmap_get_last_method_error_type(mJMAP));
}

String * JMAPSession::lastMethodErrorDescription()
{
    if (mJMAP == NULL)
        return NULL;
    return stringFromNullableCString(mailjmap_get_last_method_error_description(mJMAP));
}
