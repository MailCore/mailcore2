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

typedef num {
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

#endif
