#ifndef __MAILCORE_MCOCONSTANTS_H_

#define __MAILCORE_MCOCONSTANTS_H_

/** It's the connection type.*/
typedef enum {
    /** Clear-text connection for the protocol.*/
    MCOConnectionTypeClear             = 1 << 0,
    /** Clear-text connection at the beginning, then switch to encrypted connection using TLS/SSL*/
    /** on the same TCP connection.*/
    MCOConnectionTypeStartTLS          = 1 << 1,
    /** Encrypted connection using TLS/SSL.*/
    MCOConnectionTypeTLS               = 1 << 2,
} MCOConnectionType;

/** It's the authentication type.*/
typedef enum {
    /** Default authentication scheme of the protocol.*/
    MCOAuthTypeSASLNone          = 0,
    /** CRAM-MD5 authentication RFC 2195.*/
    MCOAuthTypeSASLCRAMMD5       = 1 << 0,
    /** PLAIN authentication RFC 4616.*/
    MCOAuthTypeSASLPlain         = 1 << 1,
    /** GSSAPI authentication.*/
    MCOAuthTypeSASLGSSAPI        = 1 << 2,
    /** DIGEST-MD5 authentication RFC 2831.*/
    MCOAuthTypeSASLDIGESTMD5     = 1 << 3,
    /** LOGIN authentication http://tools.ietf.org/id/draft-murchison-sasl-login-00.txt*/
    MCOAuthTypeSASLLogin         = 1 << 4,
    /** Secure Remote Password Authentication http://tools.ietf.org/html/draft-burdis-cat-srp-sasl-08*/
    MCOAuthTypeSASLSRP           = 1 << 5,
    /** NTLM authentication.*/
    MCOAuthTypeSASLNTLM          = 1 << 6,
    /** Kerberos 4 authentication.*/
    MCOAuthTypeSASLKerberosV4    = 1 << 7,
    /** OAuth2 authentication.*/
    MCOAuthTypeXOAuth2           = 1 << 8,
} MCOAuthType;

/** It's the IMAP flags of the folder.*/
typedef enum {
    MCOIMAPFolderFlagNone        = 0,
    /** \Marked*/
    MCOIMAPFolderFlagMarked      = 1 << 0,
    /** \Unmarked*/
    MCOIMAPFolderFlagUnmarked    = 1 << 1,
    /** \NoSelect: When a folder can't be selected.*/
    MCOIMAPFolderFlagNoSelect    = 1 << 2,
    /** \NoInferiors: When the folder has no children.*/
    MCOIMAPFolderFlagNoInferiors = 1 << 3,
    /** \Inbox: When the folder is the inbox.*/
    MCOIMAPFolderFlagInbox       = 1 << 4,
    /** \Sent: When the folder is the sent folder.*/
    MCOIMAPFolderFlagSentMail    = 1 << 5,
    /** \Starred: When the folder is the starred folder*/
    MCOIMAPFolderFlagStarred     = 1 << 6,
    /** \AllMail: When the folder is all mail.*/
    MCOIMAPFolderFlagAllMail     = 1 << 7,
    /** \Trash: When the folder is the trash.*/
    MCOIMAPFolderFlagTrash       = 1 << 8,
    /** \Drafts: When the folder is the drafts folder.*/
    MCOIMAPFolderFlagDrafts      = 1 << 9,
    /** \Spam: When the folder is the spam folder.*/
    MCOIMAPFolderFlagSpam        = 1 << 10,
    /** \Important: When the folder is the important folder.*/
    MCOIMAPFolderFlagImportant   = 1 << 11,
    /** \Archive: When the folder is archive.*/
    MCOIMAPFolderFlagArchive     = 1 << 12,
    /** \All: When the folder contains all mails, similar to \AllMail.*/
    MCOIMAPFolderFlagAll         = MCOIMAPFolderFlagAllMail,
    /** \Junk: When the folder is the spam folder.*/
    MCOIMAPFolderFlagJunk        = MCOIMAPFolderFlagSpam,
    /** \Flagged: When the folder contains all the flagged emails.*/
    MCOIMAPFolderFlagFlagged     = MCOIMAPFolderFlagStarred,
} MCOIMAPFolderFlag;

/** It's the flags of a message.*/
typedef enum {
    MCOMessageFlagNone          = 0,
    /** Seen/Read flag.*/
    MCOMessageFlagSeen          = 1 << 0,
    /** Replied/Answered flag.*/
    MCOMessageFlagAnswered      = 1 << 1,
    /** Flagged/Starred flag.*/
    MCOMessageFlagFlagged       = 1 << 2,
    /** Deleted flag.*/
    MCOMessageFlagDeleted       = 1 << 3,
    /** Draft flag.*/
    MCOMessageFlagDraft         = 1 << 4,
    /** $MDNSent flag.*/
    MCOMessageFlagMDNSent       = 1 << 5,
    /** $Forwarded flag.*/
    MCOMessageFlagForwarded     = 1 << 6,
    /** $SubmitPending flag.*/
    MCOMessageFlagSubmitPending = 1 << 7,
    /** $Submitted flag.*/
    MCOMessageFlagSubmitted     = 1 << 8,
} MCOMessageFlag;

/** It's the encoding of a part.*/
typedef enum {
    /** 7bit encoding.*/
    MCOEncoding7Bit = 0,            /** should match MAILIMAP_BODY_FLD_ENC_7BIT*/
    /** 8bit encoding.*/
    MCOEncoding8Bit = 1,            /** should match MAILIMAP_BODY_FLD_ENC_8BIT*/
    /** binary encoding.*/
    MCOEncodingBinary = 2,          /** should match MAILIMAP_BODY_FLD_ENC_BINARY*/
    /** base64 encoding.*/
    MCOEncodingBase64 = 3,          /** should match MAILIMAP_BODY_FLD_ENC_BASE64*/
    /** quoted-printable encoding.*/
    MCOEncodingQuotedPrintable = 4, /** should match MAILIMAP_BODY_FLD_ENC_QUOTED_PRINTABLE*/
    /** other encoding.*/
    MCOEncodingOther = 5,           /** should match MAILIMAP_BODY_FLD_ENC_OTHER*/
    
    /** Negative values should be used for encoding not supported by libetpan.*/
    
    /** UUEncode encoding.*/
    MCOEncodingUUEncode = -1
} MCOEncoding;

/** It's the information to fetch for a given message in the IMAP FETCH request.*/
typedef enum {
    /** UID of the message.*/
    MCOIMAPMessagesRequestKindUid            = 0, /** This is the default and it's always fetched*/
    /** Flags of the message.*/
    MCOIMAPMessagesRequestKindFlags          = 1 << 0,
    /** Headers of the message (parsed by the server).*/
    MCOIMAPMessagesRequestKindHeaders        = 1 << 1,
    /** MIME structure of the message.*/
    MCOIMAPMessagesRequestKindStructure      = 1 << 2,
    /** Received date.*/
    MCOIMAPMessagesRequestKindInternalDate   = 1 << 3,
    /** Headers through headers data.*/
    MCOIMAPMessagesRequestKindFullHeaders    = 1 << 4,
    /** Subject of the message.*/
    MCOIMAPMessagesRequestKindHeaderSubject  = 1 << 5,
    /** Gmail Labels.*/
    MCOIMAPMessagesRequestKindGmailLabels    = 1 << 6,
	/** Gmail Message ID.*/
    MCOIMAPMessagesRequestKindGmailMessageID = 1 << 7,
    /** Gmail Thread ID.*/
    MCOIMAPMessagesRequestKindGmailThreadID  = 1 << 8,
    /** Extra Headers.*/
    MCOIMAPMessagesRequestKindExtraHeaders   = 1 << 9,
} MCOIMAPMessagesRequestKind;

/** It defines the behavior of the STORE flags request.*/
typedef enum {
    /** Add the given flags.*/
    MCOIMAPStoreFlagsRequestKindAdd,
    /** Remove the given flags.*/
    MCOIMAPStoreFlagsRequestKindRemove,
    /** Set the given flags.*/
    MCOIMAPStoreFlagsRequestKindSet,
} MCOIMAPStoreFlagsRequestKind;

/** It's the search type.*/
typedef enum {
    /** No search.*/
    MCOIMAPSearchKindNone,
    /** Match sender.*/
    MCOIMAPSearchKindFrom,
    /** Match recipient.*/
    MCOIMAPSearchKindRecipient,
    /** Match subject.*/
    MCOIMAPSearchKindSubject,
    /** Match content of the message.*/
    MCOIMAPSearchKindContent,
    /** Match headers of the message.*/
    MCOIMAPSearchKindHeader,
    /** Or expresssion.*/
    MCOIMAPSearchKindOr,
    /** And expression.*/
    MCOIMAPSearchKindAnd,
} MCOIMAPSearchKind;

/** Keys for the namespace dictionary.*/
#define MCOIMAPNamespacePersonal @"IMAPNamespacePersonal"
#define MCOIMAPNamespaceOther @"IMAPNamespaceOther"
#define MCOIMAPNamespaceShared @"IMAPNamespaceShared"

/** This is the constants for the IMAP capabilities.*/
/** See corresponding RFC for more information.*/
typedef enum {
    /** ACL Capability.*/
    MCOIMAPCapabilityACL,
    /** BINARY Capability.*/
    MCOIMAPCapabilityBinary,
    /** CATENATE Capability.*/
    MCOIMAPCapabilityCatenate,
    /** CHILDREN Capability.*/
    MCOIMAPCapabilityChildren,
    /** COMPRESS Capability.*/
    MCOIMAPCapabilityCompressDeflate,
    /** CONDSTORE Capability.*/
    MCOIMAPCapabilityCondstore,
    /** ENABLE Capability.*/
    MCOIMAPCapabilityEnable,
    /** IDLE Capability.*/
    MCOIMAPCapabilityIdle,
    /** ID Capability.*/
    MCOIMAPCapabilityId,
    /** LITERAL+ Capability.*/
    MCOIMAPCapabilityLiteralPlus,
    /** MULTIAPPEND Capability.*/
    MCOIMAPCapabilityMultiAppend,
    /** NAMESPACE Capability.*/
    MCOIMAPCapabilityNamespace,
    /** QRESYNC Capability.*/
    MCOIMAPCapabilityQResync,
    /** QUOTE Capability.*/
    MCOIMAPCapabilityQuota,
    /** SORT Capability.*/
    MCOIMAPCapabilitySort,
    /** STARTTLS Capability.*/
    MCOIMAPCapabilityStartTLS,
    /** THREAD=ORDEREDSUBJECT Capability.*/
    MCOIMAPCapabilityThreadOrderedSubject,
    /** THREAD=REFERENCES Capability.*/
    MCOIMAPCapabilityThreadReferences,
    /** UIDPLUS Capability.*/
    MCOIMAPCapabilityUIDPlus,
    /** UNSELECT Capability.*/
    MCOIMAPCapabilityUnselect,
    /** XLIST Capability.*/
    MCOIMAPCapabilityXList,
    /** AUTH=ANONYMOUS Capability.*/
    MCOIMAPCapabilityAuthAnonymous,
    /** AUTH=CRAM-MD5 Capability.*/
    MCOIMAPCapabilityAuthCRAMMD5,
    /** AUTH=DIGEST-MD5 Capability.*/
    MCOIMAPCapabilityAuthDigestMD5,
    /** AUTH=EXTERNAL Capability.*/
    MCOIMAPCapabilityAuthExternal,
    /** AUTH=GSSAPI Capability.*/
    MCOIMAPCapabilityAuthGSSAPI,
    /** AUTH=KERBEROSV4 Capability.*/
    MCOIMAPCapabilityAuthKerberosV4,
    /** AUTH=LOGIN Capability.*/
    MCOIMAPCapabilityAuthLogin,
    /** AUTH=NTML Capability.*/
    MCOIMAPCapabilityAuthNTLM,
    /** AUTH=OTP Capability.*/
    MCOIMAPCapabilityAuthOTP,
    /** AUTH=PLAIN Capability.*/
    MCOIMAPCapabilityAuthPlain,
    /** AUTH=SKEY Capability.*/
    MCOIMAPCapabilityAuthSKey,
    /** AUTH=SRP Capability.*/
    MCOIMAPCapabilityAuthSRP,
    /** AUTH=XOAUTH2 Capability.*/
    MCOIMAPCapabilityXOAuth2
} MCOIMAPCapability;

/** Error domain for mailcore.*/
#define MCOErrorDomain @"MCOErrorDomain"

/** Here's the list of errors.*/
typedef enum {
    /** No error occurred.*/
    MCOErrorNone,
    /** An error related to the connection occurred.*/
    /** It could not connect or it's been disconnected.*/
    MCOErrorConnection,
    /** TLS/SSL connection was not available.*/
    MCOErrorTLSNotAvailable,
    /** The protocol could not be parsed.*/
    MCOErrorParse,
    /** Certificate was not valid.*/
    MCOErrorCertificate,
    /** An authentication error occurred.*/
    MCOErrorAuthentication,
    /** Specific to Gmail: IMAP not enabled.*/
    MCOErrorGmailIMAPNotEnabled,
    /** Specific to Gmail: Exceeded bandwidth limit.*/
    MCOErrorGmailExceededBandwidthLimit,
    /** Specific to Gmail: Too many simultaneous connections.*/
    MCOErrorGmailTooManySimultaneousConnections,
    /** Specific to Mobile Me: Moved to iCloud.*/
    MCOErrorMobileMeMoved,
    /** Specific to Yahoo: not available.*/
    MCOErrorYahooUnavailable,
    /** Non existant folder, select failed.*/
    MCOErrorNonExistantFolder,
    /** IMAP: Error occurred while renaming a folder.*/
    MCOErrorRename,
    /** IMAP: Error occurred while deleting a folder.*/
    MCOErrorDelete,
    /** IMAP: Error occurred while creating a folder.*/
    MCOErrorCreate,
    /** IMAP: Error occurred while subscribing/unsubcribing to a folder.*/
    MCOErrorSubscribe,
    /** IMAP: Error occurred while adding a message to a folder.*/
    MCOErrorAppend,
    /** IMAP: Error occurred while copying a message.*/
    MCOErrorCopy,
    /** IMAP: Error occurred while expunging.*/
    MCOErrorExpunge,
    /** IMAP: Error occurred while fetching messages.*/
    MCOErrorFetch,
    /** IMAP: Error occurred while IDLing.*/
    MCOErrorIdle,
    /** IMAP: Error occurred while sending/getting identity.*/
    MCOErrorIdentity,
    /** IMAP: Error occurred while getting namespace.*/
    MCOErrorNamespace,
    /** IMAP: Error occurred while storing flags.*/
    MCOErrorStore,
    /** IMAP: Error wile getting capabilities.*/
    MCOErrorCapability,
    /** STARTTLS is not available.*/
    MCOErrorStartTLSNotAvailable,
    /** SMTP: Illegal attachment: certain kind of attachment cannot be sent.*/
    MCOErrorSendMessageIllegalAttachment,
    /** SMTP: Storage limit: message is probably too big.*/
    MCOErrorStorageLimit,
    /** SMTP: Sending message is not allowed.*/
    MCOErrorSendMessageNotAllowed,
    /** SMTP: Specific to hotmail. Needs to connect to webmail.*/
    MCOErrorNeedsConnectToWebmail,
    /** SMTP: Error while sending message.*/
    MCOErrorSendMessage,
    /** SMTP: Authentication required.*/
    MCOErrorAuthenticationRequired,
    /** POP: Error occurred while fetching message list.*/
    MCOErrorFetchMessageList,
    /** POP: Error occurred while deleting message.*/
    MCOErrorDeleteMessage,
    /** SMTP: Error while checking account.*/
    MCOErrorInvalidAccount,
    /** IMAP: Error when trying to enable compression.*/
    MCOErrorCompression,
    /** SMTP: Error when no sender has been specified.*/
    MCOErrorNoSender,
    /** SMTP: Error when no recipient has been specified.*/
    MCOErrorNoRecipient,
    /** The count of all errors */
    MCOErrorCodeCount,
} MCOErrorCode;

/** Here's the list of connection log types.*/
typedef enum {
    /** Received data.*/
    MCOConnectionLogTypeReceived,
    /** Sent data.*/
    MCOConnectionLogTypeSent,
    /** Sent private data. It can be a password.*/
    MCOConnectionLogTypeSentPrivate,
    /** Parse error.*/
    MCOConnectionLogTypeErrorParse,
    /** Error while receiving data. The data passed to the log will be nil.*/
    MCOConnectionLogTypeErrorReceived,
    /** Error while sending dataThe data passed to the log will be nil.*/
    MCOConnectionLogTypeErrorSent,
} MCOConnectionLogType;

/**
 It's a network traffic logger.
 @param connectionID is the identifier of the underlaying network socket.
 @param type is the type of the log.
 @param data is the data related to the log.
 */
typedef void (^MCOConnectionLogger)(void * connectionID, MCOConnectionLogType type, NSData * data);

/**
 It's called when asynchronous operations stop/start running.
 */
typedef void (^MCOOperationQueueRunningChangeBlock)(void);

#endif
