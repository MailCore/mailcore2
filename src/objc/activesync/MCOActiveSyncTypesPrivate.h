#ifndef MAILCORE_MCOACTIVESYNCTYPESPRIVATE_H

#define MAILCORE_MCOACTIVESYNCTYPESPRIVATE_H

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, MCOActiveSyncFolderType) {
    MCOActiveSyncFolderTypeUnknown = 0,
    MCOActiveSyncFolderTypeUserCreatedGeneric = 1,
    MCOActiveSyncFolderTypeDefaultInbox = 2,
    MCOActiveSyncFolderTypeDefaultDrafts = 3,
    MCOActiveSyncFolderTypeDefaultDeletedItems = 4,
    MCOActiveSyncFolderTypeDefaultSentItems = 5,
    MCOActiveSyncFolderTypeDefaultOutbox = 6,
    MCOActiveSyncFolderTypeDefaultTasks = 7,
    MCOActiveSyncFolderTypeDefaultCalendar = 8,
    MCOActiveSyncFolderTypeDefaultContacts = 9,
    MCOActiveSyncFolderTypeDefaultNotes = 10,
    MCOActiveSyncFolderTypeDefaultJournal = 11,
    MCOActiveSyncFolderTypeUserCreatedMail = 12,
    MCOActiveSyncFolderTypeUserCreatedCalendar = 13,
    MCOActiveSyncFolderTypeUserCreatedContacts = 14,
    MCOActiveSyncFolderTypeUserCreatedTasks = 15,
    MCOActiveSyncFolderTypeUserCreatedJournal = 16,
    MCOActiveSyncFolderTypeUserCreatedNotes = 17,
    MCOActiveSyncFolderTypeServerUnknown = 18,
    MCOActiveSyncFolderTypeRecipientInformationCache = 19,
};

#endif
