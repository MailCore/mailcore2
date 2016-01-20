#include "MCWin32.h" // Should be included first.

#include "MCSMTPSession.h"

#include <string.h>
#include <libetpan/libetpan.h>

#include "MCAddress.h"
#include "MCMessageBuilder.h"
#include "MCMessageParser.h"
#include "MCMessageHeader.h"
#include "MCSMTPProgressCallback.h"
#include "MCConnectionLoggerUtils.h"
#include "MCCertificateUtils.h"

using namespace mailcore;

enum {
    STATE_DISCONNECTED,
    STATE_CONNECTED,
    STATE_LOGGEDIN,
};

void SMTPSession::init()
{
    mHostname = NULL;
    mPort = 0;
    mUsername = NULL;
    mPassword = NULL;
    mOAuth2Token = NULL;
    mAuthType = AuthTypeSASLNone;
    mConnectionType = ConnectionTypeClear;
    mTimeout = 30;
    mCheckCertificateEnabled = true;
    mUseHeloIPEnabled = false;
    mShouldDisconnect = false;
    
    mSmtp = NULL;
    mProgressCallback = NULL;
    mState = STATE_DISCONNECTED;
    mLastSMTPResponse = NULL;
    mLastLibetpanError = 0;
    mLastSMTPResponseCode = 0;
    mConnectionLogger = NULL;
}

SMTPSession::SMTPSession()
{
    init();
}

SMTPSession::~SMTPSession()
{
    MC_SAFE_RELEASE(mLastSMTPResponse);
    MC_SAFE_RELEASE(mHostname);
    MC_SAFE_RELEASE(mUsername);
    MC_SAFE_RELEASE(mPassword);
    MC_SAFE_RELEASE(mOAuth2Token);
}

void SMTPSession::setHostname(String * hostname)
{
    MC_SAFE_REPLACE_COPY(String, mHostname, hostname);
}

String * SMTPSession::hostname()
{
    return mHostname;
}

void SMTPSession::setPort(unsigned int port)
{
    mPort = port;
}

unsigned int SMTPSession::port()
{
    return mPort;
}

void SMTPSession::setUsername(String * username)
{
    MC_SAFE_REPLACE_COPY(String, mUsername, username);
}

String * SMTPSession::username()
{
    return mUsername;
}

void SMTPSession::setPassword(String * password)
{
    MC_SAFE_REPLACE_COPY(String, mPassword, password);
}

String * SMTPSession::password()
{
    return mPassword;
}

void SMTPSession::setOAuth2Token(String * token)
{
    MC_SAFE_REPLACE_COPY(String, mOAuth2Token, token);
}

String * SMTPSession::OAuth2Token()
{
    return mOAuth2Token;
}

void SMTPSession::setAuthType(AuthType authType)
{
    mAuthType = authType;
}

AuthType SMTPSession::authType()
{
    return mAuthType;
}

void SMTPSession::setConnectionType(ConnectionType connectionType)
{
    mConnectionType = connectionType;
}

ConnectionType SMTPSession::connectionType()
{
    return mConnectionType;
}

void SMTPSession::setTimeout(time_t timeout)
{
    mTimeout = timeout;
}

time_t SMTPSession::timeout()
{
    return mTimeout;
}

void SMTPSession::setCheckCertificateEnabled(bool enabled)
{
    mCheckCertificateEnabled = enabled;
}

bool SMTPSession::isCheckCertificateEnabled()
{
    return mCheckCertificateEnabled;
}

bool SMTPSession::checkCertificate()
{
    if (!isCheckCertificateEnabled())
        return true;
    return mailcore::checkCertificate(mSmtp->stream, hostname());
}

void SMTPSession::setUseHeloIPEnabled(bool enabled)
{
    mUseHeloIPEnabled = enabled;
}

bool SMTPSession::useHeloIPEnabled()
{
    return mUseHeloIPEnabled;
}

void SMTPSession::body_progress(size_t current, size_t maximum, void * context)
{
    SMTPSession * session;
    
    session = (SMTPSession *) context;
    session->bodyProgress((unsigned int) current, (unsigned int) maximum);
}

void SMTPSession::bodyProgress(unsigned int current, unsigned int maximum)
{
    if (mProgressCallback != NULL) {
        mProgressCallback->bodyProgress(this, current, maximum);
    }
}


static void logger(mailsmtp * smtp, int log_type, const char * buffer, size_t size, void * context)
{
    SMTPSession * session = (SMTPSession *) context;
    
    if (session->connectionLogger() == NULL)
        return;
    
    ConnectionLogType type = getConnectionType(log_type);
    if ((int) type == -1) {
        // in case of MAILSTREAM_LOG_TYPE_INFO_RECEIVED or MAILSTREAM_LOG_TYPE_INFO_SENT.
        return;
    }
    bool isBuffer = isBufferFromLogType(log_type);
    
    if (isBuffer) {
        Data * data = Data::dataWithBytes(buffer, (unsigned int) size);
        session->connectionLogger()->log(session, type, data);
    }
    else {
        session->connectionLogger()->log(session, type, NULL);
    }
}


void SMTPSession::setup()
{
    mSmtp = mailsmtp_new(0, NULL);
    mailsmtp_set_timeout(mSmtp, timeout());
    mailsmtp_set_progress_callback(mSmtp, body_progress, this);
    mailsmtp_set_logger(mSmtp, logger, this);
}

void SMTPSession::unsetup()
{
    mailsmtp_free(mSmtp);
    mSmtp = NULL;
}

void SMTPSession::connectIfNeeded(ErrorCode * pError)
{
    if (mSmtp != NULL) {
        // In case pipelining is available, libetpan will disconnect the session.
        if (mSmtp->stream == NULL) {
            mShouldDisconnect = true;
        }
    }
    if (mShouldDisconnect) {
        disconnect();
        mShouldDisconnect = false;
    }
    
    if (mState == STATE_DISCONNECTED) {
        connect(pError);
    }
    else {
        * pError = ErrorNone;
    }
}

void SMTPSession::connect(ErrorCode * pError)
{
    int r;
    
    setup();
    
    switch (mConnectionType) {
        case ConnectionTypeStartTLS:
            MCLog("connect %s %u", MCUTF8(hostname()), (unsigned int) port());
            r = mailsmtp_socket_connect(mSmtp, MCUTF8(hostname()), port());
            if (r != MAILSMTP_NO_ERROR) {
                * pError = ErrorConnection;
                goto close;
            }
            
            MCLog("init");
            if (useHeloIPEnabled()) {
                r = mailsmtp_init_with_ip(mSmtp, 1);
            } else {
                r = mailsmtp_init(mSmtp);
            }
            if (r == MAILSMTP_ERROR_STREAM) {
                * pError = ErrorConnection;
                goto close;
            }
            else if (r != MAILSMTP_NO_ERROR) {
                * pError = ErrorConnection;
                goto close;
            }
            
            MCLog("start TLS");
            r = mailsmtp_socket_starttls(mSmtp);
            if (r != MAILSMTP_NO_ERROR) {
                * pError = ErrorStartTLSNotAvailable;
                goto close;
            }
            MCLog("done");
            if (!checkCertificate()) {
                * pError = ErrorCertificate;
                goto close;
            }
            
            MCLog("init after starttls");
            if (useHeloIPEnabled()) {
                r = mailsmtp_init_with_ip(mSmtp, 1);
            } else {
                r = mailsmtp_init(mSmtp);
            }
            if (r == MAILSMTP_ERROR_STREAM) {
                * pError = ErrorConnection;
                goto close;
            }
            else if (r != MAILSMTP_NO_ERROR) {
                * pError = ErrorConnection;
                goto close;
            }
            
            break;
            
        case ConnectionTypeTLS:
            r = mailsmtp_ssl_connect(mSmtp, MCUTF8(mHostname), port());
            if (r != MAILSMTP_NO_ERROR) {
                * pError = ErrorConnection;
                goto close;
            }
            if (!checkCertificate()) {
                * pError = ErrorCertificate;
                goto close;
            }
            
            MCLog("init");
            if (useHeloIPEnabled()) {
                r = mailsmtp_init_with_ip(mSmtp, 1);
            } else {
                r = mailsmtp_init(mSmtp);
            }
            if (r == MAILSMTP_ERROR_STREAM) {
                * pError = ErrorConnection;
                goto close;
            }
            else if (r != MAILSMTP_NO_ERROR) {
                * pError = ErrorConnection;
                goto close;
            }
            
            break;
            
        default:
            r = mailsmtp_socket_connect(mSmtp, MCUTF8(hostname()), port());
            if (r != MAILSMTP_NO_ERROR) {
                * pError = ErrorConnection;
                goto close;
            }
            
            MCLog("init");
            if (useHeloIPEnabled()) {
                r = mailsmtp_init_with_ip(mSmtp, 1);
            } else {
                r = mailsmtp_init(mSmtp);
            }
            if (r == MAILSMTP_ERROR_STREAM) {
                * pError = ErrorConnection;
                goto close;
            }
            else if (r != MAILSMTP_NO_ERROR) {
                * pError = ErrorConnection;
                goto close;
            }
            
            break;
    }
    
    mailstream_low * low;
    String * identifierString;
    char * identifier;
    
    low = mailstream_get_low(mSmtp->stream);
    if (mUsername != NULL) {
        identifierString = String::stringWithUTF8Format("%s@%s:%u", MCUTF8(mUsername), MCUTF8(mHostname), port());
    }
    else {
        identifierString = String::stringWithUTF8Format("%s:%u", MCUTF8(mHostname), port());
    }
    identifier = strdup(identifierString->UTF8Characters());
    mailstream_low_set_identifier(low, identifier);
    
    mState = STATE_CONNECTED;
    * pError = ErrorNone;
    return;
    
close:
    unsetup();
}

void SMTPSession::disconnect()
{
    if (mSmtp == NULL)
        return;
    
    if (mSmtp->stream != NULL) {
        mailsmtp_quit(mSmtp);
    }
    
    unsetup();
    
    mState = STATE_DISCONNECTED;
}

void SMTPSession::loginIfNeeded(ErrorCode * pError)
{
    connectIfNeeded(pError);
    if (* pError != ErrorNone)
        return;
    
    if (mState == STATE_CONNECTED) {
        login(pError);
    }
    else {
        * pError = ErrorNone;
    }
}

void SMTPSession::login(ErrorCode * pError)
{
    int r;

    if ((authType() != AuthTypeXOAuth2) && (authType() != AuthTypeXOAuth2Outlook) &&
        ((username() == NULL) || (password() == NULL))) {
        mState = STATE_LOGGEDIN;
        * pError = ErrorNone;
        return;
    }

    if (authType() == 0) {
        if (mSmtp->auth & MAILSMTP_AUTH_DIGEST_MD5) {
            setAuthType((AuthType) (authType() | AuthTypeSASLDIGESTMD5));
        }
        else if (mSmtp->auth & MAILSMTP_AUTH_CRAM_MD5) {
            setAuthType((AuthType) (authType() | AuthTypeSASLCRAMMD5));
        }
        else if (mSmtp->auth & MAILSMTP_AUTH_GSSAPI) {
            setAuthType((AuthType) (authType() | AuthTypeSASLGSSAPI));
        }
        else if (mSmtp->auth & MAILSMTP_AUTH_SRP) {
            setAuthType((AuthType) (authType() | AuthTypeSASLSRP));
        }
        else if (mSmtp->auth & MAILSMTP_AUTH_NTLM) {
            setAuthType((AuthType) (authType() | AuthTypeSASLNTLM));
        }
        else if (mSmtp->auth & MAILSMTP_AUTH_KERBEROS_V4) {
            setAuthType((AuthType) (authType() | AuthTypeSASLKerberosV4));
        }
        else if (mSmtp->auth & MAILSMTP_AUTH_PLAIN) {
            setAuthType((AuthType) (authType() | AuthTypeSASLPlain));
        }
        else if (mSmtp->auth & MAILSMTP_AUTH_LOGIN) {
            setAuthType((AuthType) (authType() | AuthTypeSASLLogin));
        }
    }

    switch (authType()) {
        case 0:
        default:
            r = mailesmtp_auth_sasl(mSmtp, "PLAIN",
                                    MCUTF8(mHostname),
                                    NULL,
                                    NULL,
                                    MCUTF8(mUsername), MCUTF8(mUsername),
                                    MCUTF8(mPassword), NULL);
            break;
            
        case AuthTypeSASLCRAMMD5:
            r = mailesmtp_auth_sasl(mSmtp, "CRAM-MD5",
                                    MCUTF8(mHostname),
                                    NULL,
                                    NULL,
                                    MCUTF8(mUsername), MCUTF8(mUsername),
                                    MCUTF8(mPassword), NULL);
            break;
            
        case AuthTypeSASLPlain:
            r = mailesmtp_auth_sasl(mSmtp, "PLAIN",
                                    MCUTF8(mHostname),
                                    NULL,
                                    NULL,
                                    MCUTF8(mUsername), MCUTF8(mUsername),
                                    MCUTF8(mPassword), NULL);
            break;
            
        case AuthTypeSASLGSSAPI:
            // needs to be tested
            r = mailesmtp_auth_sasl(mSmtp, "GSSAPI",
                                    MCUTF8(mHostname),
                                    NULL,
                                    NULL,
                                    MCUTF8(mUsername), MCUTF8(mUsername),
                                    MCUTF8(mPassword), NULL);
            break;
            
        case AuthTypeSASLDIGESTMD5:
            r = mailesmtp_auth_sasl(mSmtp, "DIGEST-MD5",
                                    MCUTF8(mHostname),
                                    NULL,
                                    NULL,
                                    MCUTF8(mUsername), MCUTF8(mUsername),
                                    MCUTF8(mPassword), NULL);
            break;
            
        case AuthTypeSASLLogin:
            r = mailesmtp_auth_sasl(mSmtp, "LOGIN",
                                    MCUTF8(mHostname),
                                    NULL,
                                    NULL,
                                    MCUTF8(mUsername), MCUTF8(mUsername),
                                    MCUTF8(mPassword), NULL);
            break;
            
        case AuthTypeSASLSRP:
            r = mailesmtp_auth_sasl(mSmtp, "SRP",
                                    MCUTF8(mHostname),
                                    NULL,
                                    NULL,
                                    MCUTF8(mUsername), MCUTF8(mUsername),
                                    MCUTF8(mPassword), NULL);
            break;
            
        case AuthTypeSASLNTLM:
            r = mailesmtp_auth_sasl(mSmtp, "NTLM",
                                    MCUTF8(mHostname),
                                    NULL,
                                    NULL,
                                    MCUTF8(mUsername), MCUTF8(mUsername),
                                    MCUTF8(mPassword), NULL /* realm */);
            break;
            
        case AuthTypeSASLKerberosV4:
            r = mailesmtp_auth_sasl(mSmtp, "KERBEROS_V4",
                                    MCUTF8(mHostname),
                                    NULL,
                                    NULL,
                                    MCUTF8(mUsername), MCUTF8(mUsername),
                                    MCUTF8(mPassword), NULL /* realm */);
            break;
            
        case AuthTypeXOAuth2: {
            const char * utf8Username = MCUTF8(mUsername);
            if (utf8Username == NULL) {
                utf8Username = "";
            }
            
            if (mOAuth2Token == NULL) {
                r = MAILSMTP_ERROR_STREAM;
            }
            else {
                r = mailsmtp_oauth2_authenticate(mSmtp, utf8Username, MCUTF8(mOAuth2Token));
            }
            break;
        }
        
        case AuthTypeXOAuth2Outlook: {
            const char * utf8Username = MCUTF8(mUsername);
            if (utf8Username == NULL) {
                utf8Username = "";
            }
            
            if (mOAuth2Token == NULL) {
                r = MAILSMTP_ERROR_STREAM;
            } 
            else {
                r = mailsmtp_oauth2_outlook_authenticate(mSmtp, utf8Username, MCUTF8(mOAuth2Token));
            }
            break;
        }
    }
    if (r == MAILSMTP_ERROR_STREAM) {
        * pError = ErrorConnection;
        mShouldDisconnect = true;
        return;
    }
    else if (r != MAILSMTP_NO_ERROR) {
        * pError = ErrorAuthentication;
        return;
    }
    
    mState = STATE_LOGGEDIN;
    * pError = ErrorNone;
}

void SMTPSession::checkAccount(Address * from, ErrorCode * pError)
{
    int r;
    
    loginIfNeeded(pError);
    if (* pError != ErrorNone) {
        return;
    }
    r = mailsmtp_mail(mSmtp, MCUTF8(from->mailbox()));
    if (r == MAILSMTP_ERROR_STREAM) {
        * pError = ErrorConnection;
        mShouldDisconnect = true;
        return;
    }
    else if (r != MAILSMTP_NO_ERROR) {
        * pError = ErrorInvalidAccount;
        return;
    }
    
    r = mailsmtp_rcpt(mSmtp, "email@invalid.com");
    if (r == MAILSMTP_ERROR_STREAM) {
        * pError = ErrorConnection;
        mShouldDisconnect = true;
        return;
    }
    else if (r != MAILSMTP_NO_ERROR) {
        * pError = ErrorInvalidAccount;
        return;
    }
    
    * pError = ErrorNone;
}

void SMTPSession::sendMessage(Address * from, Array * recipients, Data * messageData,
    SMTPProgressCallback * callback, ErrorCode * pError)
{
    clist * address_list;
    int r;

    if (from == NULL) {
        * pError = ErrorNoSender;
        return;
    }
    if ((recipients == NULL) || (recipients->count() == 0)) {
        * pError = ErrorNoRecipient;
        return;
    }
    
    messageData = dataWithFilteredBcc(messageData);
    
    mProgressCallback = callback;
    bodyProgress(0, messageData->length());
    
    MCLog("setup");

    MCLog("connect");
    loginIfNeeded(pError);
    if (* pError != ErrorNone) {
        goto err;
    }

    // disable DSN feature for more compatibility
    mSmtp->esmtp &= ~MAILSMTP_ESMTP_DSN;

    address_list = esmtp_address_list_new();
    for(unsigned int i = 0 ; i < recipients->count() ; i ++) {
        Address * addr = (Address *) recipients->objectAtIndex(i);
        esmtp_address_list_add(address_list, (char *) MCUTF8(addr->mailbox()), 0, NULL);
    }
    MCLog("send");
    if ((mSmtp->esmtp & MAILSMTP_ESMTP_PIPELINING) != 0) {
        r = mailesmtp_send_quit(mSmtp, MCUTF8(from->mailbox()), 0, NULL,
            address_list,
            messageData->bytes(), messageData->length());
        mShouldDisconnect = true;
    }
    else {
        r = mailesmtp_send(mSmtp, MCUTF8(from->mailbox()), 0, NULL,
            address_list,
            messageData->bytes(), messageData->length());
        mailsmtp_quit(mSmtp);
    }
    esmtp_address_list_free(address_list);

    String * response;
    int responseCode;

    response = NULL;
    if (mSmtp->response != NULL) {
        response = String::stringWithUTF8Characters(mSmtp->response);
    }
    responseCode = mSmtp->response_code;

    if ((r == MAILSMTP_ERROR_STREAM) || (r == MAILSMTP_ERROR_CONNECTION_REFUSED)) {
        * pError = ErrorConnection;
        mShouldDisconnect = true;
        goto err;
    }
    else if (r == MAILSMTP_ERROR_EXCEED_STORAGE_ALLOCATION) {
        if ((response != NULL) && (response->locationOfString(MCSTR("5.7.0")) != -1)) {
            * pError = ErrorSendMessageIllegalAttachment;
        }
        else {
            * pError = ErrorStorageLimit;
        }
        goto err;
    }
    else if (r == MAILSMTP_ERROR_MAILBOX_UNAVAILABLE) {
        * pError = ErrorSendMessageNotAllowed;
        goto err;
    }
    else if (r == MAILSMTP_ERROR_AUTH_REQUIRED) {
        * pError = ErrorAuthenticationRequired;
        goto err;
    }
    else if (r != MAILSMTP_NO_ERROR) {
        if ((responseCode == 550) && (response != NULL)) {
            if (response->hasPrefix(MCSTR("5.3.4 "))) {
                * pError = ErrorNeedsConnectToWebmail;
                goto err;
            }
            else if (response->hasPrefix(MCSTR("5.7.1 "))) {
                * pError = ErrorSendMessageNotAllowed;
                goto err;
            }
        }
        
        * pError = ErrorSendMessage;
        MC_SAFE_REPLACE_COPY(String, mLastSMTPResponse, response);
        mLastLibetpanError = r;
        mLastSMTPResponseCode = responseCode;
        goto err;
    }

    bodyProgress(messageData->length(), messageData->length());
    * pError = ErrorNone;
    
    err:
    mProgressCallback = NULL;
}

Data * SMTPSession::dataWithFilteredBcc(Data * data)
{
    int r;
    size_t idx;
    struct mailimf_message * msg;
    MMAPString * str;
    
    idx = 0;
    r = mailimf_message_parse(data->bytes(), data->length(), &idx, &msg);
    if (r != MAILIMF_NO_ERROR) {
        return Data::data();
    }
    
    struct mailimf_fields * fields = msg->msg_fields;
    int col = 0;

    int hasRecipient = 0;
    str = mmap_string_new("");
    for(clistiter * cur = clist_begin(fields->fld_list) ; cur != NULL ; cur = clist_next(cur)) {
        struct mailimf_field * field = (struct mailimf_field *) clist_content(cur);
        if (field->fld_type == MAILIMF_FIELD_BCC) {
            mailimf_field_free(field);
            clist_delete(fields->fld_list, cur);
            break;
        }
        else if ((field->fld_type == MAILIMF_FIELD_TO) || (field->fld_type == MAILIMF_FIELD_CC) || (field->fld_type == MAILIMF_FIELD_BCC)) {
            hasRecipient = 1;
        }
    }
    if (!hasRecipient) {
        struct mailimf_address_list * imfTo;
        imfTo = mailimf_address_list_new_empty();
        mailimf_address_list_add_parse(imfTo, (char *) "Undisclosed recipients:;");
        struct mailimf_to * toField = mailimf_to_new(imfTo);
        struct mailimf_field * field = mailimf_field_new(MAILIMF_FIELD_TO, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, toField, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        mailimf_fields_add(fields, field);
    }
    mailimf_fields_write_mem(str, &col, fields);
    mmap_string_append(str, "\n");
    mmap_string_append_len(str, msg->msg_body->bd_text, msg->msg_body->bd_size);
    
    Data * result = Data::dataWithBytes(str->str, (unsigned int) str->len);
    
    mmap_string_free(str);
    mailimf_message_free(msg);
    
    return result;
}

void SMTPSession::sendMessage(Data * messageData, SMTPProgressCallback * callback, ErrorCode * pError)
{
    AutoreleasePool * pool = new AutoreleasePool();
    MessageParser * parser = new MessageParser(messageData);
    Array * recipients = new Array();
    
    if (parser->header()->to() != NULL) {
        recipients->addObjectsFromArray(parser->header()->to());
    }
    if (parser->header()->cc() != NULL) {
        recipients->addObjectsFromArray(parser->header()->cc());
    }
    if (parser->header()->bcc() != NULL) {
        recipients->addObjectsFromArray(parser->header()->bcc());
    }
    Address * from = parser->header()->from();
    
    sendMessage(from, recipients, messageData, callback, pError);
    
    recipients->release();
    parser->release();
    pool->release();
}

void SMTPSession::sendMessage(MessageBuilder * msg, SMTPProgressCallback * callback, ErrorCode * pError)
{
    Array * recipients = new Array();
    if (msg->header()->to() != NULL) {
        recipients->addObjectsFromArray(msg->header()->to());
    }
    if (msg->header()->cc() != NULL) {
        recipients->addObjectsFromArray(msg->header()->cc());
    }
    if (msg->header()->bcc() != NULL) {
        recipients->addObjectsFromArray(msg->header()->bcc());
    }
    Address * from = msg->header()->from();
    Data * data = msg->data();
    MCAssert(from != NULL);
    MCAssert(recipients->count() > 0);
    sendMessage(from, recipients, data, callback, pError);
    recipients->release();
}

void SMTPSession::noop(ErrorCode * pError)
{
    int r;
    
    if (mSmtp == NULL)
        return;
    
    MCLog("connect");
    loginIfNeeded(pError);
    if (* pError != ErrorNone) {
        return;
    }
    if (mSmtp->stream != NULL) {
        r = mailsmtp_noop(mSmtp);
        if (r == MAILSMTP_ERROR_STREAM) {
            * pError = ErrorConnection;
        }
    }
}

bool SMTPSession::isDisconnected()
{
    return mState == STATE_DISCONNECTED;
}

void SMTPSession::setConnectionLogger(ConnectionLogger * logger)
{
    mConnectionLogger = logger;
}

ConnectionLogger * SMTPSession::connectionLogger()
{
    return mConnectionLogger;
}
