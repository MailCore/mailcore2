#ifndef __MAILCORE_MCMESSAGECONSTANTS_H_
#define __MAILCORE_MCMESSAGECONSTANTS_H_

#ifdef __cplusplus

namespace mailcore {
    
    enum ConnectionType {
        ConnectionTypeClear             = 1 << 0,
        ConnectionTypeStartTLS          = 1 << 1,
        ConnectionTypeTLS               = 1 << 2,
    };

    enum AuthType {
        AuthTypeSASLNone          = 0,
        AuthTypeSASLCRAMMD5       = 1 << 0,
        AuthTypeSASLPlain         = 1 << 1,
        AuthTypeSASLGSSAPI        = 1 << 2,
        AuthTypeSASLDIGESTMD5     = 1 << 3,
        AuthTypeSASLLogin         = 1 << 4,
        AuthTypeSASLSRP           = 1 << 5,
        AuthTypeSASLNTLM          = 1 << 6,
        AuthTypeSASLKerberosV4    = 1 << 7,
        AuthTypeXOAuth2           = 1 << 8,
    };

    enum IMAPFolderFlag {
        IMAPFolderFlagNone        = 0,
        IMAPFolderFlagMarked      = 1 << 0,
        IMAPFolderFlagUnmarked    = 1 << 1,
        IMAPFolderFlagNoSelect    = 1 << 2,
        IMAPFolderFlagNoInferiors = 1 << 3,
        IMAPFolderFlagInbox       = 1 << 4,
        IMAPFolderFlagSentMail    = 1 << 5,
        IMAPFolderFlagStarred     = 1 << 6,
        IMAPFolderFlagAllMail     = 1 << 7,
        IMAPFolderFlagTrash       = 1 << 8,
        IMAPFolderFlagDrafts      = 1 << 9,
        IMAPFolderFlagSpam        = 1 << 10,
        IMAPFolderFlagImportant   = 1 << 11,
        IMAPFolderFlagArchive     = 1 << 12,
        IMAPFolderFlagAll = IMAPFolderFlagAllMail,
        IMAPFolderFlagJunk = IMAPFolderFlagSpam,
        IMAPFolderFlagFlagged = IMAPFolderFlagStarred,
    };

    enum MessageFlag {
        MessageFlagNone          = 0,
        MessageFlagSeen          = 1 << 0,
        MessageFlagAnswered      = 1 << 1,
        MessageFlagFlagged       = 1 << 2,
        MessageFlagDeleted       = 1 << 3,
        MessageFlagDraft         = 1 << 4,
        MessageFlagMDNSent       = 1 << 5,
        MessageFlagForwarded     = 1 << 6,
        MessageFlagSubmitPending = 1 << 7,
        MessageFlagSubmitted     = 1 << 8,
    } ;

    enum IMAPMessagesRequestKind {
        IMAPMessagesRequestKindUid           = 0, // This is the default and it's always fetched
        IMAPMessagesRequestKindFlags         = 1 << 0,
        IMAPMessagesRequestKindHeaders       = 1 << 1,
        IMAPMessagesRequestKindStructure     = 1 << 2,
        IMAPMessagesRequestKindInternalDate  = 1 << 3,
        IMAPMessagesRequestKindFullHeaders   = 1 << 4,
        IMAPMessagesRequestKindHeaderSubject = 1 << 5,
        IMAPMessagesRequestKindGmailLabels   = 1 << 6,
        IMAPMessagesRequestKindGmailMessageID = 1 << 7,
        IMAPMessagesRequestKindGmailThreadID  = 1 << 8,
    };

    enum IMAPFetchRequestType {
        IMAPFetchRequestTypeUID = 0,
        IMAPFetchRequestTypeSequence = 1
    };

    enum IMAPStoreFlagsRequestKind {
        IMAPStoreFlagsRequestKindAdd,
        IMAPStoreFlagsRequestKindRemove,
        IMAPStoreFlagsRequestKindSet,
    };

    enum IMAPWorkaround {
        IMAPWorkaroundGmail = 1 << 0,
        IMAPWorkaroundYahoo = 1 << 1,
        IMAPWorkaroundExchange2003 = 1 << 2,
    };

    enum IMAPCapability {
        IMAPCapabilityACL,
        IMAPCapabilityBinary,
        IMAPCapabilityCatenate,
        IMAPCapabilityChildren,
        IMAPCapabilityCompressDeflate,
        IMAPCapabilityCondstore,
        IMAPCapabilityEnable,
        IMAPCapabilityIdle,
        IMAPCapabilityId,
        IMAPCapabilityLiteralPlus,
        IMAPCapabilityMultiAppend,
        IMAPCapabilityNamespace,
        IMAPCapabilityQResync,
        IMAPCapabilityQuota,
        IMAPCapabilitySort,
        IMAPCapabilityStartTLS,
        IMAPCapabilityThreadOrderedSubject,
        IMAPCapabilityThreadReferences,
        IMAPCapabilityUIDPlus,
        IMAPCapabilityUnselect,
        IMAPCapabilityXList,
        IMAPCapabilityAuthAnonymous,
        IMAPCapabilityAuthCRAMMD5,
        IMAPCapabilityAuthDigestMD5,
        IMAPCapabilityAuthExternal,
        IMAPCapabilityAuthGSSAPI,
        IMAPCapabilityAuthKerberosV4,
        IMAPCapabilityAuthLogin,
        IMAPCapabilityAuthNTLM,
        IMAPCapabilityAuthOTP,
        IMAPCapabilityAuthPlain,
        IMAPCapabilityAuthSKey,
        IMAPCapabilityAuthSRP,
        IMAPCapabilityXOAuth2,
    };

    enum POPCapability {
        POPCapabilityNone,
        POPCapabilityStartTLS,
        POPCapabilityTop,
        POPCapabilityUser,
        POPCapabilityRespCodes,
        POPCapabilityPipelining,
        POPCapabilityUIDL,
        POPCapabilitySASL,
        POPCapabilityAuthAnonymous,
        POPCapabilityAuthCRAMMD5,
        POPCapabilityAuthDigestMD5,
        POPCapabilityAuthExternal,
        POPCapabilityAuthGSSAPI,
        POPCapabilityAuthKerberosV4,
        POPCapabilityAuthLogin,
        POPCapabilityAuthNTLM,
        POPCapabilityAuthOTP,
        POPCapabilityAuthPlain,
        POPCapabilityAuthSKey,
        POPCapabilityAuthSRP,
    };

    enum Encoding {
        Encoding7Bit = 0,            // should match MAILIMAP_BODY_FLD_ENC_7BIT
        Encoding8Bit = 1,            // should match MAILIMAP_BODY_FLD_ENC_8BIT
        EncodingBinary = 2,          // should match MAILIMAP_BODY_FLD_ENC_BINARY
        EncodingBase64 = 3,          // should match MAILIMAP_BODY_FLD_ENC_BASE64
        EncodingQuotedPrintable = 4, // should match MAILIMAP_BODY_FLD_ENC_QUOTED_PRINTABLE
        EncodingOther = 5,           // should match MAILIMAP_BODY_FLD_ENC_OTHER
        // negative values should be used for other encoding
        EncodingUUEncode = -1
    };
    
    enum IMAPSearchKind {
        IMAPSearchKindNone,
        IMAPSearchKindFrom,
        IMAPSearchKindRecipient,
        IMAPSearchKindSubject,
        IMAPSearchKindContent,
        IMAPSearchKindHeader,
        IMAPSearchKindOr,
        IMAPSearchKindAnd,
    };
    
    enum ErrorCode {
        ErrorNone,
        ErrorConnection,
        ErrorTLSNotAvailable,
        ErrorParse,
        ErrorCertificate,
        ErrorAuthentication,
        ErrorGmailIMAPNotEnabled,
        ErrorGmailExceededBandwidthLimit,
        ErrorGmailTooManySimultaneousConnections,
        ErrorMobileMeMoved,
        ErrorYahooUnavailable,
        ErrorNonExistantFolder,
        ErrorRename,
        ErrorDelete,
        ErrorCreate,
        ErrorSubscribe,
        ErrorAppend,
        ErrorCopy,
        ErrorExpunge,
        ErrorFetch,
        ErrorIdle,
        ErrorIdentity,
        ErrorNamespace,
        ErrorStore,
        ErrorCapability,
        ErrorStartTLSNotAvailable,
        ErrorSendMessageIllegalAttachment,
        ErrorStorageLimit,
        ErrorSendMessageNotAllowed,
        ErrorNeedsConnectToWebmail,
        ErrorSendMessage,
        ErrorAuthenticationRequired,
        ErrorFetchMessageList,
        ErrorDeleteMessage,
        ErrorInvalidAccount,
        ErrorFile,
    };
    
    enum PartType {
        PartTypeSingle,
        PartTypeMessage,
        PartTypeMultipartMixed,
        PartTypeMultipartRelated,
        PartTypeMultipartAlternative,
    };
    
    // Private type - It should not be used directly.
    enum IMAPMessageRenderingType {
        IMAPMessageRenderingTypeHTML,
        IMAPMessageRenderingTypeHTMLBody,
        IMAPMessageRenderingTypePlainText,
        IMAPMessageRenderingTypePlainTextBody
    };
}

#endif

#endif