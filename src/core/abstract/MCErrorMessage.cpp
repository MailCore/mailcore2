#include "MCErrorMessage.h"

using namespace mailcore;

static const char * localizedDescriptionTable[] = {
    "The operation succeeded.", /** MCOErrorNone */
    "A stable connection to the server could not be established.",                   /** MCOErrorConnection */
    "The server does not support TLS/SSL connections.",                              /** MCOErrorTLSNotAvailable */
    "Unable to parse response from server.",                                         /** MCOErrorParse */
    "The certificate for this server is invalid.",                                   /** MCOErrorCertificate */
    "Unable to authenticate with the current session's credentials.",                /** MCOErrorAuthentication */
    "IMAP is not enabled for this Gmail account.",                                   /** MCOErrorGmailIMAPNotEnabled */
    "Bandwidth limits were exceeded while requesting data from this Gmail account.", /** MCOErrorGmailExceededBandwidthLimit */
    "Too many simultaneous connections were made to this Gmail account.",            /** MCOErrorGmailTooManySimultaneousConnections */
    "MobileMe is no longer an active mail service.",                                 /** MCOErrorMobileMeMoved */
    "Yahoo!'s servers are currently unavailable.",                                   /** MCOErrorYahooUnavailable */
    "The requested folder does not exist.  Folder selection failed",                 /** MCOErrorNonExistantFolder */
    "An error occured while renaming the requested folder.",                         /** MCOErrorRename */
    "An error occured while deleting the requested folder.",                         /** MCOErrorDelete */
    "An error occured while creating the requested folder.",                         /** MCOErrorCreate */
    "An error occured while (un)subscribing to the requested folder.",               /** MCOErrorSubscribe */
    "An error occured while appending a message to the requested folder.",           /** MCOErrorAppend */
    "An error occured while copying a message to the requested folder.",             /** MCOErrorCopy */
    "An error occured while expunging a message in the requested folder.",           /** MCOErrorExpunge */
    "An error occured while fetching messages in the requested folder.",             /** MCOErrorFetch */
    "An error occured during an IDLE operation.",                                    /** MCOErrorIdle */
    "An error occured while requesting the server's identity.",                      /** MCOErrorIdentity */
    "An error occured while requesting the server's namespace.",                     /** MCOErrorNamespace */
    "An error occured while storing flags.",                                         /** MCOErrorStore */
    "An error occured while requesting the server's capabilities.",                  /** MCOErrorCapability */
    "The server does not support STARTTLS connections.",                             /** MCOErrorStartTLSNotAvailable */
    "Attempted to send a message with an illegal attachment.",                       /** MCOErrorSendMessageIllegalAttachment */
    "The SMTP storage limit was hit while trying to send a large message.",          /** MCOErrorStorageLimit */
    "Sending messages is not allowed on this server.",                               /** MCOErrorSendMessageNotAllowed */
    "The current HotMail account cannot connect to WebMail.",                        /** MCOErrorNeedsConnectToWebmail */
    "An error occured while sending the message.",                                   /** MCOErrorSendMessage */
    "Authentication is required for this SMTP server.",                              /** MCOErrorAuthenticationRequired */
    "An error occured while fetching a message list on the POP server.",             /** MCOErrorFetchMessageList */
    "An error occured while deleting a message on the POP server.",                  /** MCOErrorDeleteMessage */
    "Account check failed because the account is invalid.",                          /** MCOErrorInvalidAccount */
    "File access error",                                                             /** MCOErrorFile */
    "Compression is not available",                                                  /** MCOErrorCompression */
    "A sender is required to send message",                                          /** MCOErrorNoSender */
    "A recipient is required to send message",                                       /** MCOErrorNoRecipient */
    "An error occured while performing a No-Op operation.",                          /** MCOErrorNoop */
    "An application specific password is required",                                  /** MCOErrorGmailApplicationSpecificPasswordRequired */
    "An error when requesting date",                                                 /** MCOErrorServerDate */
    "No valid server found",                                                         /** MCOErrorNoValidServerFound */
};

String * mailcore::errorMessageWithErrorCode(ErrorCode errorCode)
{
    if (errorCode < 0) {
        return NULL;
    }
    if (errorCode >= sizeof(localizedDescriptionTable) / sizeof(localizedDescriptionTable[0])) {
        return NULL;
    }
    return String::stringWithUTF8Characters(localizedDescriptionTable[errorCode]);
}
