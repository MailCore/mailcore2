#ifndef __MAILCORE_MCOCONSTANTS_H_

#define __MAILCORE_MCOCONSTANTS_H_

typedef enum {
    MCOConnectionTypeClear             = 1 << 0,
    MCOConnectionTypeStartTLS          = 1 << 1,
    MCOConnectionTypeTLS               = 1 << 2,
} MCOConnectionType;

typedef enum {
    MCOAuthTypeSASLNone          = 0,
    MCOAuthTypeSASLCRAMMD5       = 1 << 0,
    MCOAuthTypeSASLPlain         = 1 << 1,
    MCOAuthTypeSASLGSSAPI        = 1 << 2,
    MCOAuthTypeSASLDIGESTMD5     = 1 << 3,
    MCOAuthTypeSASLLogin         = 1 << 4,
    MCOAuthTypeSASLSRP           = 1 << 5,
    MCOAuthTypeSASLNTLM          = 1 << 6,
    MCOAuthTypeSASLKerberosV4    = 1 << 7,
} MCOAuthType;

typedef enum {
    MCOIMAPFolderFlagNone        = 0,
    MCOIMAPFolderFlagMarked      = 1 << 0,
    MCOIMAPFolderFlagUnmarked    = 1 << 1,
    MCOIMAPFolderFlagNoSelect    = 1 << 2,
    MCOIMAPFolderFlagNoInferiors = 1 << 3,
    MCOIMAPFolderFlagInbox       = 1 << 4,
    MCOIMAPFolderFlagSentMail    = 1 << 5,
    MCOIMAPFolderFlagStarred     = 1 << 6,
    MCOIMAPFolderFlagAllMail     = 1 << 7,
    MCOIMAPFolderFlagTrash       = 1 << 8,
    MCOIMAPFolderFlagDrafts      = 1 << 9,
    MCOIMAPFolderFlagSpam        = 1 << 10,
    MCOIMAPFolderFlagImportant   = 1 << 11,
    MCOIMAPFolderFlagArchive     = 1 << 12,
} MCOIMAPFolderFlag;

typedef enum {
    MCOMessageFlagNone          = 0,
    MCOMessageFlagSeen          = 1 << 0,
    MCOMessageFlagAnswered      = 1 << 1,
    MCOMessageFlagFlagged       = 1 << 2,
    MCOMessageFlagDeleted       = 1 << 3,
    MCOMessageFlagDraft         = 1 << 4,
    MCOMessageFlagMDNSent       = 1 << 5,
    MCOMessageFlagForwarded     = 1 << 6,
    MCOMessageFlagSubmitPending = 1 << 7,
    MCOMessageFlagSubmitted     = 1 << 8,
} MCOMessageFlag;

typedef enum {
    Encoding7Bit = 0,            // should match MAILIMAP_BODY_FLD_ENC_7BIT
    Encoding8Bit = 1,            // should match MAILIMAP_BODY_FLD_ENC_8BIT
    EncodingBinary = 2,          // should match MAILIMAP_BODY_FLD_ENC_BINARY
    EncodingBase64 = 3,          // should match MAILIMAP_BODY_FLD_ENC_BASE64
    EncodingQuotedPrintable = 4, // should match MAILIMAP_BODY_FLD_ENC_QUOTED_PRINTABLE
    EncodingOther = 5,           // should match MAILIMAP_BODY_FLD_ENC_OTHER
    // negative values should be used for other encoding
    EncodingUUEncode = -1
} MCOEncoding;

#endif
