//
//  MCNNTPSession.cpp
//  mailcore2
//
//  Created by Robert Widmann on 3/6/14.
//  Copyright (c) 2014 MailCore. All rights reserved.
//

#include "MCNNTPSession.h"

#include <string.h>
#include <libetpan/libetpan.h>

#include "MCNNTPGroupInfo.h"
#include "MCMessageHeader.h"
#include "MCConnectionLoggerUtils.h"
#include "MCCertificateUtils.h"

#define NNTP_DEFAULT_PORT  119
#define NNTPS_DEFAULT_PORT 563

using namespace mailcore;

enum {
    STATE_DISCONNECTED,
    STATE_CONNECTED,
    STATE_LOGGEDIN,
    STATE_LISTED,
    STATE_SELECTED,
};

void NNTPSession::init()
{
    mHostname = NULL;
    mPort = NNTP_DEFAULT_PORT;
    mUsername = NULL;
    mPassword = NULL;
    mConnectionType = ConnectionTypeClear;
    mCheckCertificateEnabled = true;
    mTimeout = 30;
    
    mNNTP = NULL;
    mState = STATE_DISCONNECTED;
    mConnectionLogger = NULL;
}

NNTPSession::NNTPSession()
{
    init();
}

NNTPSession::~NNTPSession()
{
    MC_SAFE_RELEASE(mHostname);
    MC_SAFE_RELEASE(mUsername);
    MC_SAFE_RELEASE(mPassword);
}

void NNTPSession::setHostname(String * hostname)
{
    MC_SAFE_REPLACE_COPY(String, mHostname, hostname);
}

String * NNTPSession::hostname()
{
    return mHostname;
}

void NNTPSession::setPort(unsigned int port)
{
    mPort = port;
}

unsigned int NNTPSession::port()
{
    return mPort;
}

void NNTPSession::setUsername(String * username)
{
    MC_SAFE_REPLACE_COPY(String, mUsername, username);
}

String * NNTPSession::username()
{
    return mUsername;
}

void NNTPSession::setPassword(String * password)
{
    MC_SAFE_REPLACE_COPY(String, mPassword, password);
}

String * NNTPSession::password()
{
    return mPassword;
}

void NNTPSession::setConnectionType(ConnectionType connectionType)
{
    mConnectionType = connectionType;
}

ConnectionType NNTPSession::connectionType()
{
    return mConnectionType;
}

void NNTPSession::setTimeout(time_t timeout)
{
    mTimeout = timeout;
}

time_t NNTPSession::timeout()
{
    return mTimeout;
}

void NNTPSession::setCheckCertificateEnabled(bool enabled)
{
    mCheckCertificateEnabled = enabled;
}

bool NNTPSession::isCheckCertificateEnabled()
{
    return mCheckCertificateEnabled;
}

bool NNTPSession::checkCertificate()
{
    if (!isCheckCertificateEnabled())
        return true;
    return mailcore::checkCertificate(mNNTP->nntp_stream, hostname());
}

static void logger(newsnntp * nntp, int log_type, const char * buffer, size_t size, void * context)
{
    NNTPSession * session = (NNTPSession *) context;
    
    if (session->connectionLogger() == NULL)
        return;
    
    ConnectionLogType type = getConnectionType(log_type);
    bool isBuffer = isBufferFromLogType(log_type);
    
    if (isBuffer) {
        Data * data = Data::dataWithBytes(buffer, (unsigned int) size);
        session->connectionLogger()->log(session, type, data);
    }
    else {
        session->connectionLogger()->log(session, type, NULL);
    }
}


void NNTPSession::setup()
{
    mNNTP = newsnntp_new(0, NULL);
    newsnntp_set_logger(mNNTP, logger, this);
}

void NNTPSession::unsetup()
{
    if (mNNTP != NULL) {
        if (mNNTP->nntp_stream != NULL) {
            mailstream_close(mNNTP->nntp_stream);
            mNNTP->nntp_stream = NULL;
        }
        newsnntp_free(mNNTP);
        mNNTP = NULL;
    }
}

void NNTPSession::loginIfNeeded(ErrorCode * pError)
{
    connectIfNeeded(pError);
    if (* pError != ErrorNone) {
        return;
    }
    
    if (mState == STATE_CONNECTED) {
        login(pError);
    }
    else {
        * pError = ErrorNone;
    }
}

void NNTPSession::readerIfNeeded(ErrorCode * pError)
{
    connectIfNeeded(pError);
    if (* pError != ErrorNone)
        return;
    
    if (mState == STATE_CONNECTED) {
        newsnntp_mode_reader(mNNTP);
    }
    else {
        * pError = ErrorNone;
    }
}

void NNTPSession::login(ErrorCode * pError)
{
    int r;

    if (mUsername != NULL) {
        r = newsnntp_authinfo_username(mNNTP, mUsername->UTF8Characters());
        if (r == NEWSNNTP_ERROR_STREAM) {
            * pError = ErrorConnection;
            return;
        }
        else if (r != NEWSNNTP_NO_ERROR) {
            * pError = ErrorAuthentication;
            return;
        }
    }
    if (mPassword != NULL) {
        r = newsnntp_authinfo_password(mNNTP, mPassword->UTF8Characters());
        if (r == NEWSNNTP_ERROR_STREAM) {
            * pError = ErrorConnection;
            return;
        }
        else if (r != NEWSNNTP_NO_ERROR) {
            * pError = ErrorAuthentication;
            return;
        }
    }

    mState = STATE_LOGGEDIN;
    * pError = ErrorNone;
}

void NNTPSession::connect(ErrorCode * pError)
{
    int r;
    
    setup();
    
    switch (mConnectionType) {
        case ConnectionTypeStartTLS:
            MCLog("connect %s %u", MCUTF8(hostname()), (unsigned int) port());
            r = newsnntp_socket_connect(mNNTP, MCUTF8(hostname()), port());
            if (r != NEWSNNTP_NO_ERROR) {
                * pError = ErrorConnection;
                return;
            }
            MCLog("done");
            if (!checkCertificate()) {
                * pError = ErrorCertificate;
                return;
            }
            break;
            
        case ConnectionTypeTLS:
            MCLog("connect %s %u", MCUTF8(hostname()), (unsigned int) port());
            r = newsnntp_ssl_connect(mNNTP, MCUTF8(hostname()), port());
            if (r != NEWSNNTP_NO_ERROR) {
                * pError = ErrorConnection;
                return;
            }
            if (!checkCertificate()) {
                * pError = ErrorCertificate;
                return;
            }
            break;
            
        default:
            r = newsnntp_socket_connect(mNNTP, MCUTF8(hostname()), port());
            if (r != NEWSNNTP_NO_ERROR) {
                * pError = ErrorConnection;
                return;
            }
            break;
    }
    
    mailstream_low * low;
    String * identifierString;
    char * identifier;
    
    low = mailstream_get_low(mNNTP->nntp_stream);
    if (mUsername != NULL) {
        identifierString = String::stringWithUTF8Format("%s@%s:%u", MCUTF8(mUsername), MCUTF8(mHostname), mPort);
    }
    else {
        identifierString = String::stringWithUTF8Format("%s:%u", MCUTF8(mUsername), mPort);
    }
    identifier = strdup(identifierString->UTF8Characters());
    mailstream_low_set_identifier(low, identifier);
    mState = STATE_CONNECTED;
    * pError = ErrorNone;
}

void NNTPSession::connectIfNeeded(ErrorCode * pError)
{
    if (mState == STATE_DISCONNECTED) {
        connect(pError);
    }
    else {
        * pError = ErrorNone;
    }
}

void NNTPSession::disconnect()
{
    if (mNNTP == NULL)
        return;
    
    newsnntp_quit(mNNTP);
    mState = STATE_DISCONNECTED;
    unsetup();
}

void NNTPSession::checkAccount(ErrorCode * pError)
{
    loginIfNeeded(pError);
}

Array * NNTPSession::listAllNewsgroups(ErrorCode * pError)
{
    int r;
    clist * grp_list;
    
    loginIfNeeded(pError);
    if (* pError != ErrorNone) {
        return NULL;
    }
    
    r = newsnntp_list(mNNTP, &grp_list);
    if (r == NEWSNNTP_ERROR_STREAM) {
        * pError = ErrorConnection;
        return NULL;
    }
    else if (r != NEWSNNTP_NO_ERROR) {
        * pError = ErrorFetchMessageList;
        return NULL;
    }
    
    Array * result = Array::array();
    clistiter * iter;
    for(iter = clist_begin(grp_list) ;iter != NULL ; iter = clist_next(iter)) {
        struct newsnntp_group_info * grp_info;
        String * name;
        
        grp_info = (struct newsnntp_group_info *) clist_content(iter);
        
        name = String::stringWithUTF8Characters(grp_info->grp_name);
        
        NNTPGroupInfo * info = new NNTPGroupInfo();
        info->setName(name);
        result->addObject(info);
        info->release();
    }
    
    newsnntp_list_free(grp_list);
    * pError = ErrorNone;
    mState = STATE_LISTED;
    
    return result;
}

Array * NNTPSession::listSubscribedNewsgroups(ErrorCode * pError)
{
    int r;
    clist * subd_groups;
    
    MCLog("fetch subscribed");
    loginIfNeeded(pError);
    if (* pError != ErrorNone)
        return NULL;

    r = newsnntp_list_subscriptions(mNNTP, &subd_groups);
    MCLog("fetch subscribed %u", r);
    
    Array * result = Array::array();
    clistiter * iter;
    for(iter = clist_begin(subd_groups) ;iter != NULL ; iter = clist_next(iter)) {
        struct newsnntp_group_info * grp_info;
        String * name;
        
        grp_info = (struct newsnntp_group_info *) clist_content(iter);
        
        name = String::stringWithUTF8Characters(strdup(grp_info->grp_name));
        name->retain();
        
        NNTPGroupInfo * info = new NNTPGroupInfo();
        info->setName(name);
        result->addObject(info);
        info->release();
    }
    newsnntp_list_subscriptions_free(subd_groups);
    * pError = ErrorNone;

    return result;
}

MessageHeader * NNTPSession::fetchHeader(String *groupName, unsigned int index, ErrorCode * pError) 
{
    int r;
    char * content;
    size_t content_len;
    
    MCLog("fetch header at index %u", index);
    loginIfNeeded(pError);
    if (* pError != ErrorNone) {
        return NULL;
    }
    
    selectGroup(groupName, pError);
    if (* pError != ErrorNone) {
        return NULL;
    }
    
    r = newsnntp_head(mNNTP, index, &content, &content_len);
    if (r != NEWSNNTP_NO_ERROR) {
        * pError = ErrorFetchMessageList;
        return NULL;
    }
    
    Data * data;
    data = new Data(content, (unsigned int) content_len);
    MessageHeader * result = new MessageHeader();
    result->importHeadersData(data);
    result->autorelease();
    data->release();
    
    newsnntp_head_free(content);
    * pError = ErrorNone;
    
    return result;
}

Data * NNTPSession::fetchArticle(String *groupName, unsigned int index, NNTPProgressCallback * callback, ErrorCode * pError) 
{
    int r;
    char * content;
    size_t content_len;
    
    MCLog("fetch article at index %u", index);
    
    loginIfNeeded(pError);
    if (* pError != ErrorNone) {
        return NULL;
    }
    
    selectGroup(groupName, pError);
    if (* pError != ErrorNone) {
        return NULL;
    }
    
    r = newsnntp_article(mNNTP, index, &content, &content_len);
    if (r == NEWSNNTP_ERROR_STREAM) {
        * pError = ErrorConnection;
        return NULL;
    }
    else if (r != NEWSNNTP_NO_ERROR) {
        * pError = ErrorFetchMessageList;
        return NULL;
    }
    
    Data * result;
    result = Data::dataWithBytes(content, (unsigned int) content_len);
    newsnntp_article_free(content);
    * pError = ErrorNone;
    
    return result;
}

IndexSet * NNTPSession::fetchArticles(String * groupName, ErrorCode * pError) 
{
    int r;
    clist * msg_list;
    
    selectGroup(groupName, pError);
    if (* pError != ErrorNone) {
        return NULL;
    }
    
    r = newsnntp_listgroup(mNNTP, groupName->UTF8Characters(), &msg_list);
    if (r == NEWSNNTP_ERROR_STREAM) {
        * pError = ErrorConnection;
        return NULL;
    }
    else if (r != NEWSNNTP_NO_ERROR) {
        * pError = ErrorFetchMessageList;
        return NULL;
    }
    
    IndexSet * result = new IndexSet();
    clistiter * iter;
    for(iter = clist_begin(msg_list) ;iter != NULL ; iter = clist_next(iter)) {
        uint32_t *msg_info;
        
        msg_info = (uint32_t *) clist_content(iter);
        if (!msg_info) {
            continue;
        }
        
        result->addIndex(*msg_info);
    }
    
    newsnntp_listgroup_free(msg_list);
    * pError = ErrorNone;
    mState = STATE_LISTED;
    
    return result;
}

void NNTPSession::selectGroup(String * folder, ErrorCode * pError) 
{
    int r;
    struct newsnntp_group_info * info;
    
    readerIfNeeded(pError);
    if (* pError != ErrorNone) {
        return;
    }
    
    r = newsnntp_group(mNNTP, folder->UTF8Characters(), &info);
    if (r == NEWSNNTP_ERROR_STREAM) {
        * pError = ErrorConnection;
        MCLog("select error : %s %i", MCUTF8DESC(this), * pError);
        return;
    }
    else if (r == NEWSNNTP_ERROR_NO_SUCH_NEWS_GROUP) {
        * pError = ErrorNonExistantFolder;
        return;
    }
    else if (r == MAILIMAP_ERROR_PARSE) {
        * pError = ErrorParse;
        return;
    }
    
    mState = STATE_SELECTED;
    * pError = ErrorNone;
    MCLog("select ok");
}

void NNTPSession::setConnectionLogger(ConnectionLogger * logger)
{
    mConnectionLogger = logger;
}

ConnectionLogger * NNTPSession::connectionLogger()
{
    return mConnectionLogger;
}
