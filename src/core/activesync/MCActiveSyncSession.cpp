#include "MCActiveSync.h"

#include "MCActiveSyncPrivate.h"

#include <libetpan/clist.h>
#include <libetpan/mailactivesync.h>

#include "MCMessageHeader.h"

#include <stdlib.h>
#include <string.h>

using namespace mailcore;

static String * stringFromCString(const char * value)
{
    if (value == NULL)
        return NULL;
    return String::stringWithUTF8Characters(value);
}

static Data * dataFromBytes(const char * bytes, size_t length)
{
    if (bytes == NULL)
        return NULL;
    return Data::dataWithBytes(bytes, (unsigned int) length);
}

static const char * cString(String * value)
{
    if (value == NULL)
        return NULL;
    return value->UTF8Characters();
}

static void appendHeaderField(Data * data, const char * name, const char * value)
{
    if (value == NULL)
        return;
    data->appendBytes(name, (unsigned int) strlen(name));
    data->appendBytes(": ", 2);
    data->appendBytes(value, (unsigned int) strlen(value));
    data->appendBytes("\r\n", 2);
}

static void importActiveSyncMessageHeader(ActiveSyncMessage * message,
    struct mailactivesync_message * native)
{
    Data * MIMEData = message->MIMEData();
    if (MIMEData != NULL && MIMEData->length() > 0) {
        message->header()->importHeadersData(MIMEData);
        return;
    }

    Data * headerData = Data::data();
    appendHeaderField(headerData, "Subject", native->subject);
    appendHeaderField(headerData, "From", native->from);
    appendHeaderField(headerData, "To", native->to);
    appendHeaderField(headerData, "Cc", native->cc);
    appendHeaderField(headerData, "Reply-To", native->reply_to);
    appendHeaderField(headerData, "Date", native->date_received);
    headerData->appendBytes("\r\n", 2);
    message->header()->importHeadersData(headerData);
}

static ErrorCode errorFromActiveSync(int error)
{
    switch (error) {
        case MAILACTIVESYNC_NO_ERROR:
            return ErrorNone;
        case MAILACTIVESYNC_ERROR_UNAUTHORIZED:
        case MAILACTIVESYNC_ERROR_CLIENT_DENIED:
            return ErrorAuthentication;
        case MAILACTIVESYNC_ERROR_PARSE:
        case MAILACTIVESYNC_ERROR_PROTOCOL:
        case MAILACTIVESYNC_ERROR_RESPONSE_NOT_WBXML:
            return ErrorParse;
        case MAILACTIVESYNC_ERROR_SSL:
            return ErrorCertificate;
        case MAILACTIVESYNC_ERROR_MEMORY:
        case MAILACTIVESYNC_ERROR_SERVER_BUSY:
        case MAILACTIVESYNC_ERROR_HTTP_UNAVAILABLE:
            return ErrorStorageLimit;
        case MAILACTIVESYNC_ERROR_NOT_IMPLEMENTED:
            return ErrorCustomCommand;
        case MAILACTIVESYNC_ERROR_BAD_STATE:
        case MAILACTIVESYNC_ERROR_STREAM:
        case MAILACTIVESYNC_ERROR_HTTP:
        case MAILACTIVESYNC_ERROR_REDIRECT:
        default:
            return ErrorConnection;
    }
}

static void setError(ErrorCode * pError, int activeSyncError)
{
    if (pError != NULL)
        * pError = errorFromActiveSync(activeSyncError);
}

static Array * /* String */ stringArrayFromClist(clist * list)
{
    Array * /* String */ result = Array::array();
    for (clistiter * cur = clist_begin(list); cur != NULL; cur = clist_next(cur)) {
        String * value = stringFromCString((char *) clist_content(cur));
        if (value != NULL)
            result->addObject(value);
    }
    return result;
}

static ActiveSyncAttachment * attachmentFromNative(struct mailactivesync_attachment * native)
{
    if (native == NULL)
        return NULL;

    ActiveSyncAttachment * result = new ActiveSyncAttachment();
    result->setMethod((ActiveSyncAttachmentMethod) native->method);
    result->setContentID(stringFromCString(native->content_id));
    result->setContentLocation(stringFromCString(native->content_location));
    result->setInlineAttachment(native->is_inline != 0);
    result->setMimeType(stringFromCString(native->content_type));
    result->setFilename(stringFromCString(native->display_name));
    result->setAttachment(native->is_inline == 0);
    result->setUniqueID(stringFromCString(native->file_reference));
    result->setEstimatedDataSize(native->estimated_data_size);
    return (ActiveSyncAttachment *) result->autorelease();
}

static ActiveSyncBody * bodyFromNative(struct mailactivesync_airsyncbase_body * native)
{
    if (native == NULL)
        return NULL;

    ActiveSyncBody * result = new ActiveSyncBody();
    result->setType((ActiveSyncBodyType) native->type);
    result->setData(dataFromBytes(native->data, native->data_len));
    result->setEstimatedDataSize(native->estimated_data_size);
    result->setTruncated(native->truncated != 0);
    result->setNativeBodyType((ActiveSyncBodyType) native->native_body_type);
    result->setMimeType(stringFromCString(native->content_type));
    result->setPreview(stringFromCString(native->preview));

    Array * /* ActiveSyncAttachment */ attachments = Array::array();
    for (clistiter * cur = clist_begin(native->attachments); cur != NULL; cur = clist_next(cur)) {
        ActiveSyncAttachment * attachment = attachmentFromNative((struct mailactivesync_attachment *) clist_content(cur));
        if (attachment != NULL)
            attachments->addObject(attachment);
    }
    result->setAttachments(attachments);
    return (ActiveSyncBody *) result->autorelease();
}

static ActiveSyncMessage * messageFromNative(struct mailactivesync_message * native)
{
    if (native == NULL)
        return NULL;

    ActiveSyncMessage * result = new ActiveSyncMessage();
    result->setServerID(stringFromCString(native->server_id));
    result->setMessageClass(stringFromCString(native->message_class));
    result->setEstimatedSize(native->estimated_size);
    result->setRead(native->read != 0);
    result->setFlagged(native->flagged != 0);
    result->setMIMEData(dataFromBytes(native->mime, native->mime_len));
    result->setBody(bodyFromNative(native->body));
    importActiveSyncMessageHeader(result, native);
    return (ActiveSyncMessage *) result->autorelease();
}

static ActiveSyncMessage * messageFromNativeItem(struct mailactivesync_item * native)
{
    if (native == NULL)
        return NULL;

    ActiveSyncMessage * result = new ActiveSyncMessage();
    result->setServerID(stringFromCString(native->server_id));
    result->setMIMEData(dataFromBytes(native->mime, native->mime_len));
    result->setBody(bodyFromNative(native->body));
    if (result->MIMEData() != NULL && result->MIMEData()->length() > 0)
        result->header()->importHeadersData(result->MIMEData());
    return (ActiveSyncMessage *) result->autorelease();
}

static ActiveSyncFolder * folderFromNative(struct mailactivesync_folder * native)
{
    if (native == NULL)
        return NULL;

    ActiveSyncFolder * result = new ActiveSyncFolder();
    result->setServerID(stringFromCString(native->server_id));
    result->setParentID(stringFromCString(native->parent_id));
    result->setDisplayName(stringFromCString(native->display_name));
    result->setType((ActiveSyncFolderType) native->type);
    return (ActiveSyncFolder *) result->autorelease();
}

static Array * /* ActiveSyncFolder */ folderArrayFromClist(clist * list)
{
    Array * /* ActiveSyncFolder */ result = Array::array();
    for (clistiter * cur = clist_begin(list); cur != NULL; cur = clist_next(cur)) {
        ActiveSyncFolder * folder = folderFromNative((struct mailactivesync_folder *) clist_content(cur));
        if (folder != NULL)
            result->addObject(folder);
    }
    return result;
}

static Array * /* ActiveSyncMessage */ messageArrayFromClist(clist * list)
{
    Array * /* ActiveSyncMessage */ result = Array::array();
    for (clistiter * cur = clist_begin(list); cur != NULL; cur = clist_next(cur)) {
        ActiveSyncMessage * message = messageFromNative((struct mailactivesync_message *) clist_content(cur));
        if (message != NULL)
            result->addObject(message);
    }
    return result;
}

void ActiveSyncSession::init()
{
    mServerURL = NULL;
    mUsername = NULL;
    mPassword = NULL;
    mOAuth2Token = NULL;
    mDeviceID = NULL;
    mDeviceType = NULL;
    mProtocolVersion = NULL;
    mPolicyKey = NULL;
    mUserAgent = NULL;
    mCached = false;
    mCacheDirectory = NULL;
    mSession = NULL;
}

ActiveSyncSession::ActiveSyncSession()
{
    init();
}

ActiveSyncSession::~ActiveSyncSession()
{
    if (mSession != NULL)
        mailactivesync_free(mSession);
    MC_SAFE_RELEASE(mServerURL);
    MC_SAFE_RELEASE(mUsername);
    MC_SAFE_RELEASE(mPassword);
    MC_SAFE_RELEASE(mOAuth2Token);
    MC_SAFE_RELEASE(mDeviceID);
    MC_SAFE_RELEASE(mDeviceType);
    MC_SAFE_RELEASE(mProtocolVersion);
    MC_SAFE_RELEASE(mPolicyKey);
    MC_SAFE_RELEASE(mUserAgent);
    MC_SAFE_RELEASE(mCacheDirectory);
}

void ActiveSyncSession::setServerURL(String * value)
{
    MC_SET_STRING_FIELD(mServerURL, value);
}

String * ActiveSyncSession::serverURL()
{
    MC_GET_STRING_FIELD(mServerURL);
}

void ActiveSyncSession::setUsername(String * value)
{
    MC_SET_STRING_FIELD(mUsername, value);
}

String * ActiveSyncSession::username()
{
    MC_GET_STRING_FIELD(mUsername);
}

void ActiveSyncSession::setPassword(String * value)
{
    MC_SET_STRING_FIELD(mPassword, value);
}

String * ActiveSyncSession::password()
{
    MC_GET_STRING_FIELD(mPassword);
}

void ActiveSyncSession::setOAuth2Token(String * value)
{
    MC_SET_STRING_FIELD(mOAuth2Token, value);
}

String * ActiveSyncSession::OAuth2Token()
{
    MC_GET_STRING_FIELD(mOAuth2Token);
}

void ActiveSyncSession::setDeviceID(String * value)
{
    MC_SET_STRING_FIELD(mDeviceID, value);
}

String * ActiveSyncSession::deviceID()
{
    MC_GET_STRING_FIELD(mDeviceID);
}

void ActiveSyncSession::setDeviceType(String * value)
{
    MC_SET_STRING_FIELD(mDeviceType, value);
}

String * ActiveSyncSession::deviceType()
{
    MC_GET_STRING_FIELD(mDeviceType);
}

void ActiveSyncSession::setProtocolVersion(String * value)
{
    MC_SET_STRING_FIELD(mProtocolVersion, value);
}

String * ActiveSyncSession::protocolVersion()
{
    MC_GET_STRING_FIELD(mProtocolVersion);
}

void ActiveSyncSession::setPolicyKey(String * value)
{
    MC_SET_STRING_FIELD(mPolicyKey, value);
}

String * ActiveSyncSession::policyKey()
{
    MC_GET_STRING_FIELD(mPolicyKey);
}

void ActiveSyncSession::setUserAgent(String * value)
{
    MC_SET_STRING_FIELD(mUserAgent, value);
}

String * ActiveSyncSession::userAgent()
{
    MC_GET_STRING_FIELD(mUserAgent);
}

void ActiveSyncSession::setCached(bool value)
{
    mCached = value;
}

bool ActiveSyncSession::isCached()
{
    return mCached;
}

void ActiveSyncSession::setCacheDirectory(String * value)
{
    MC_SET_STRING_FIELD(mCacheDirectory, value);
}

String * ActiveSyncSession::cacheDirectory()
{
    MC_GET_STRING_FIELD(mCacheDirectory);
}

void ActiveSyncSession::ensureSession()
{
    if (mSession == NULL)
        mSession = mailactivesync_new(mCached ? 1 : 0, cString(mCacheDirectory));
}

void ActiveSyncSession::configureSession(ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return;
    }

    int result = MAILACTIVESYNC_NO_ERROR;
    if (mDeviceID != NULL || mDeviceType != NULL)
        result = mailactivesync_set_device(mSession, cString(mDeviceID), cString(mDeviceType));
    if (result == MAILACTIVESYNC_NO_ERROR && mProtocolVersion != NULL)
        result = mailactivesync_set_protocol_version(mSession, cString(mProtocolVersion));
    if (result == MAILACTIVESYNC_NO_ERROR && mPolicyKey != NULL)
        result = mailactivesync_set_policy_key(mSession, cString(mPolicyKey));
    if (result == MAILACTIVESYNC_NO_ERROR && mUserAgent != NULL)
        result = mailactivesync_set_user_agent(mSession, cString(mUserAgent));

    setError(pError, result);
}

String * ActiveSyncSession::lastRedirectURL()
{
    ensureSession();
    return stringFromCString(mailactivesync_get_last_redirect_url(mSession));
}

String * ActiveSyncSession::lastAuthenticateHeader()
{
    ensureSession();
    return stringFromCString(mailactivesync_get_last_authenticate_header(mSession));
}

void ActiveSyncSession::connect(ErrorCode * pError)
{
    configureSession(pError);
    if (pError != NULL && * pError != ErrorNone)
        return;

    setError(pError, mailactivesync_connect(mSession, cString(mServerURL)));
}

void ActiveSyncSession::login(ErrorCode * pError)
{
    connect(pError);
    if (pError != NULL && * pError != ErrorNone)
        return;

    setError(pError, mailactivesync_login(mSession, cString(mUsername), cString(mPassword)));
}

void ActiveSyncSession::loginOAuth2(ErrorCode * pError)
{
    connect(pError);
    if (pError != NULL && * pError != ErrorNone)
        return;

    setError(pError, mailactivesync_login_oauth2(mSession, cString(mUsername), cString(mOAuth2Token)));
}

void ActiveSyncSession::setOAuth2TokenOnConnection(ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return;
    }

    setError(pError, mailactivesync_set_oauth2_token(mSession, cString(mOAuth2Token)));
}

ActiveSyncOptions * ActiveSyncSession::options(ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return NULL;
    }

    struct mailactivesync_options * native = NULL;
    int resultCode = mailactivesync_options(mSession, &native);
    setError(pError, resultCode);
    if (resultCode != MAILACTIVESYNC_NO_ERROR)
        return NULL;

    ActiveSyncOptions * result = new ActiveSyncOptions();
    result->setProtocolVersions(stringArrayFromClist(native->protocol_versions));
    result->setCommands(stringArrayFromClist(native->commands));
    mailactivesync_options_free(native);
    return (ActiveSyncOptions *) result->autorelease();
}

ActiveSyncFolderSyncResult * ActiveSyncSession::folderSync(String * syncKey, ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return NULL;
    }

    struct mailactivesync_folder_sync_result * native = NULL;
    int resultCode = mailactivesync_folder_sync(mSession, cString(syncKey), &native);
    setError(pError, resultCode);
    if (resultCode != MAILACTIVESYNC_NO_ERROR)
        return NULL;

    ActiveSyncFolderSyncResult * result = new ActiveSyncFolderSyncResult();
    result->setSyncKey(stringFromCString(native->sync_key));
    result->setStatus((ActiveSyncFolderSyncStatus) native->status);
    result->setAdded(folderArrayFromClist(native->added));
    result->setUpdated(folderArrayFromClist(native->updated));
    result->setDeleted(stringArrayFromClist(native->deleted));
    mailactivesync_folder_sync_result_free(native);
    return (ActiveSyncFolderSyncResult *) result->autorelease();
}

ActiveSyncSyncResult * ActiveSyncSession::sync(ActiveSyncSyncRequest * request, ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return NULL;
    }

    struct mailactivesync_sync_request * nativeRequest = mailactivesync_sync_request_new(cString(request->collectionID()), cString(request->syncKey()));
    if (nativeRequest == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return NULL;
    }

    if (request->collectionClass() != NULL)
        mailactivesync_sync_request_set_collection_class(nativeRequest, cString(request->collectionClass()));
    mailactivesync_sync_request_set_get_changes(nativeRequest, request->getChanges() ? 1 : 0);
    mailactivesync_sync_request_set_deletes_as_moves(nativeRequest, request->deletesAsMoves() ? 1 : 0);
    if (request->hasFilterType())
        mailactivesync_sync_request_set_filter_type(nativeRequest, request->filterType());
    if (request->hasConflict())
        mailactivesync_sync_request_set_conflict(nativeRequest, request->conflict());
    if (request->windowSize() != 0)
        mailactivesync_sync_request_set_window_size(nativeRequest, request->windowSize());
    if (request->hasBodyPreference())
        mailactivesync_sync_request_set_body_preference(nativeRequest, request->bodyPreferenceType(), request->bodyPreferenceTruncationSize());

    struct mailactivesync_sync_result * native = NULL;
    int resultCode = mailactivesync_sync(mSession, nativeRequest, &native);
    mailactivesync_sync_request_free(nativeRequest);
    setError(pError, resultCode);
    if (resultCode != MAILACTIVESYNC_NO_ERROR)
        return NULL;

    ActiveSyncSyncResult * result = new ActiveSyncSyncResult();
    result->setSyncKey(stringFromCString(native->sync_key));
    result->setStatus((ActiveSyncSyncStatus) native->status);
    result->setMoreAvailable(native->more_available != 0);
    result->setEmptyResponse(native->empty_response != 0);
    result->setSyncKeyFromResponse(native->sync_key_from_response != 0);
    result->setAdded(messageArrayFromClist(native->added));
    result->setChanged(messageArrayFromClist(native->changed));
    result->setDeleted(stringArrayFromClist(native->deleted));
    mailactivesync_sync_result_free(native);
    return (ActiveSyncSyncResult *) result->autorelease();
}

ActiveSyncProvisionResult * ActiveSyncSession::provision(ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return NULL;
    }

    struct mailactivesync_provision_result * native = NULL;
    int resultCode = mailactivesync_provision(mSession, &native);
    setError(pError, resultCode);
    if (resultCode != MAILACTIVESYNC_NO_ERROR)
        return NULL;

    ActiveSyncProvisionResult * result = new ActiveSyncProvisionResult();
    result->setStatus((ActiveSyncProvisionStatus) native->status);
    result->setPolicyStatus((ActiveSyncProvisionPolicyStatus) native->policy_status);
    result->setPolicyKey(stringFromCString(native->policy_key));
    mailactivesync_provision_result_free(native);
    return (ActiveSyncProvisionResult *) result->autorelease();
}

ActiveSyncSettingsResult * ActiveSyncSession::setDeviceInformation(HashMap * /* String -> String */ deviceInformation, ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return NULL;
    }

    struct mailactivesync_device_information info;
    memset(&info, 0, sizeof(info));
    info.model = cString((String *) deviceInformation->objectForKey(MCSTR("model")));
    info.imei = cString((String *) deviceInformation->objectForKey(MCSTR("imei")));
    info.friendly_name = cString((String *) deviceInformation->objectForKey(MCSTR("friendlyName")));
    info.os = cString((String *) deviceInformation->objectForKey(MCSTR("os")));
    info.os_language = cString((String *) deviceInformation->objectForKey(MCSTR("osLanguage")));
    info.phone_number = cString((String *) deviceInformation->objectForKey(MCSTR("phoneNumber")));
    info.user_agent = cString((String *) deviceInformation->objectForKey(MCSTR("userAgent")));
    info.mobile_operator = cString((String *) deviceInformation->objectForKey(MCSTR("mobileOperator")));

    struct mailactivesync_settings_result * native = NULL;
    int resultCode = mailactivesync_settings_set_device_information(mSession, &info, &native);
    setError(pError, resultCode);
    if (resultCode != MAILACTIVESYNC_NO_ERROR)
        return NULL;

    ActiveSyncSettingsResult * result = new ActiveSyncSettingsResult();
    result->setStatus((ActiveSyncSettingsStatus) native->status);
    result->setDeviceInformationStatus((ActiveSyncSettingsDeviceInformationStatus) native->device_information_status);
    mailactivesync_settings_result_free(native);
    return (ActiveSyncSettingsResult *) result->autorelease();
}

ActiveSyncItemEstimateResult * ActiveSyncSession::itemEstimate(String * collectionID, String * syncKey, ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return NULL;
    }

    struct mailactivesync_get_item_estimate_result * native = NULL;
    int resultCode = mailactivesync_get_item_estimate(mSession, cString(collectionID), cString(syncKey), &native);
    setError(pError, resultCode);
    if (resultCode != MAILACTIVESYNC_NO_ERROR)
        return NULL;

    ActiveSyncItemEstimateResult * result = new ActiveSyncItemEstimateResult();
    result->setStatus((ActiveSyncItemEstimateStatus) native->status);
    result->setCollectionStatus((ActiveSyncItemEstimateStatus) native->collection_status);
    result->setEstimate(native->estimate);
    result->setEmptyResponse(native->empty_response != 0);
    mailactivesync_get_item_estimate_result_free(native);
    return (ActiveSyncItemEstimateResult *) result->autorelease();
}

ActiveSyncMessage * ActiveSyncSession::fetchItem(String * collectionID, String * serverID, ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return NULL;
    }

    struct mailactivesync_item * native = NULL;
    int resultCode = mailactivesync_item_operations_fetch(mSession, cString(collectionID), cString(serverID), &native);
    setError(pError, resultCode);
    if (resultCode != MAILACTIVESYNC_NO_ERROR)
        return NULL;

    ActiveSyncMessage * result = messageFromNativeItem(native);
    mailactivesync_item_free(native);
    return result;
}

void ActiveSyncSession::sendMail(Data * MIMEData, bool saveInSent, ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return;
    }

    setError(pError, mailactivesync_send_mail(mSession, MIMEData->bytes(), MIMEData->length(), saveInSent ? 1 : 0));
}

void ActiveSyncSession::smartReply(String * collectionID, String * serverID, Data * MIMEData, bool saveInSent, ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return;
    }

    setError(pError, mailactivesync_smart_reply(mSession, cString(collectionID), cString(serverID), MIMEData->bytes(), MIMEData->length(), saveInSent ? 1 : 0));
}

void ActiveSyncSession::smartForward(String * collectionID, String * serverID, Data * MIMEData, bool saveInSent, ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return;
    }

    setError(pError, mailactivesync_smart_forward(mSession, cString(collectionID), cString(serverID), MIMEData->bytes(), MIMEData->length(), saveInSent ? 1 : 0));
}

ActiveSyncPingResult * ActiveSyncSession::ping(Array * /* String */ collectionIDs, uint32_t heartbeatInterval, ErrorCode * pError)
{
    ensureSession();
    if (mSession == NULL) {
        if (pError != NULL)
            * pError = ErrorStorageLimit;
        return NULL;
    }

    struct mailactivesync_ping_request request;
    memset(&request, 0, sizeof(request));
    request.heartbeat_interval = heartbeatInterval;
    request.collection_ids = clist_new();
    for (unsigned int i = 0; i < collectionIDs->count(); i++) {
        String * collectionID = (String *) collectionIDs->objectAtIndex(i);
        clist_append(request.collection_ids, strdup(collectionID->UTF8Characters()));
    }

    struct mailactivesync_ping_result * native = NULL;
    int resultCode = mailactivesync_ping(mSession, &request, &native);
    clist_foreach(request.collection_ids, (clist_func) free, NULL);
    clist_free(request.collection_ids);
    setError(pError, resultCode);
    if (resultCode != MAILACTIVESYNC_NO_ERROR)
        return NULL;

    ActiveSyncPingResult * result = new ActiveSyncPingResult();
    result->setStatus((ActiveSyncPingStatus) native->status);
    result->setChangedCollectionIDs(stringArrayFromClist(native->changed_collection_ids));
    mailactivesync_ping_result_free(native);
    return (ActiveSyncPingResult *) result->autorelease();
}
