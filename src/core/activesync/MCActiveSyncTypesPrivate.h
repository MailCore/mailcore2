#ifndef MAILCORE_MCACTIVESYNCTYPESPRIVATE_H

#define MAILCORE_MCACTIVESYNCTYPESPRIVATE_H

#ifdef __cplusplus

namespace mailcore {

    enum ActiveSyncFolderType {
        ActiveSyncFolderTypeUnknown = 0,
        ActiveSyncFolderTypeUserCreatedGeneric = 1,
        ActiveSyncFolderTypeDefaultInbox = 2,
        ActiveSyncFolderTypeDefaultDrafts = 3,
        ActiveSyncFolderTypeDefaultDeletedItems = 4,
        ActiveSyncFolderTypeDefaultSentItems = 5,
        ActiveSyncFolderTypeDefaultOutbox = 6,
        ActiveSyncFolderTypeDefaultTasks = 7,
        ActiveSyncFolderTypeDefaultCalendar = 8,
        ActiveSyncFolderTypeDefaultContacts = 9,
        ActiveSyncFolderTypeDefaultNotes = 10,
        ActiveSyncFolderTypeDefaultJournal = 11,
        ActiveSyncFolderTypeUserCreatedMail = 12,
        ActiveSyncFolderTypeUserCreatedCalendar = 13,
        ActiveSyncFolderTypeUserCreatedContacts = 14,
        ActiveSyncFolderTypeUserCreatedTasks = 15,
        ActiveSyncFolderTypeUserCreatedJournal = 16,
        ActiveSyncFolderTypeUserCreatedNotes = 17,
        ActiveSyncFolderTypeServerUnknown = 18,
        ActiveSyncFolderTypeRecipientInformationCache = 19,
    };

}

#endif

#endif
