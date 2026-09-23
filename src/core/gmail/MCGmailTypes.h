#ifndef MAILCORE_MCGMAILTYPES_H

#define MAILCORE_MCGMAILTYPES_H

#ifdef __cplusplus

namespace mailcore {

    enum GmailMessageFormat {
        GmailMessageFormatMinimal,
        GmailMessageFormatFull,
        GmailMessageFormatRaw,
        GmailMessageFormatMetadata,
    };

}

#endif

#endif
